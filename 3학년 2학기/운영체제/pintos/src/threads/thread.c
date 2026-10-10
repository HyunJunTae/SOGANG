#include "threads/thread.h"
#include <debug.h>
#include <stddef.h>
#include <random.h>
#include <stdio.h>
#include <string.h>
#include "threads/flags.h"
#include "threads/interrupt.h"
#include "threads/intr-stubs.h"
#include "threads/palloc.h"
#include "threads/switch.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#ifdef USERPROG
#include "userprog/process.h"
#endif

// advanced scheduler 여기 구현
#include "threads/fixed_point.h"
#include "devices/timer.h"

/* Random value for struct thread's `magic' member.
   Used to detect stack overflow.  See the big comment at the top
   of thread.h for details. */
#define THREAD_MAGIC 0xcd6abf4b

/* List of processes in THREAD_READY state, that is, processes
   that are ready to run but not actually running. */
static struct list ready_list;

/* List of all processes.  Processes are added to this list
   when they are first scheduled and removed when they exit. */
static struct list all_list;

// alarm clock 여기 구현
/* List of threads that are sleeping. */
static struct list sleeping_thread_list;

/* Idle thread. */
static struct thread *idle_thread;

/* Initial thread, the thread running init.c:main(). */
static struct thread *initial_thread;

/* Lock used by allocate_tid(). */
static struct lock tid_lock;

/* Stack frame for kernel_thread(). */
struct kernel_thread_frame 
  {
    void *eip;                  /* Return address. */
    thread_func *function;      /* Function to call. */
    void *aux;                  /* Auxiliary data for function. */
  };

/* Statistics. */
static long long idle_ticks;    /* # of timer ticks spent idle. */
static long long kernel_ticks;  /* # of timer ticks in kernel threads. */
static long long user_ticks;    /* # of timer ticks in user programs. */

/* Scheduling. */
#define TIME_SLICE 4            /* # of timer ticks to give each thread. */
static unsigned thread_ticks;   /* # of timer ticks since last yield. */

/* If false (default), use round-robin scheduler.
   If true, use multi-level feedback queue scheduler.
   Controlled by kernel command-line option "-o mlfqs". */
bool thread_mlfqs;

// advanced scheduler 여기 구현
// 1. 시스템 전체의 부하 평균을 저장하는 변수
int load_avg;

#ifndef USERPROG
/* Project #3. */
bool thread_prior_aging;
#endif

static void kernel_thread (thread_func *, void *aux);

static void idle (void *aux UNUSED);
static struct thread *running_thread (void);
static struct thread *next_thread_to_run (void);
static void init_thread (struct thread *, const char *name, int priority);
static bool is_thread (struct thread *) UNUSED;
static void *alloc_frame (struct thread *, size_t size);
static void schedule (void);
void thread_schedule_tail (struct thread *prev);
static tid_t allocate_tid (void);

// priority scheduling 여기 구현
bool cmp_priority (const struct list_elem *a, const struct list_elem *b, void *aux UNUSED)
{
  // 1. list_elem 포인터를 이용해 해당 요소가 속한 실제 thread 구조체를 가져온다.
  // 2. 첫 번째 인자(a)의 우선순위가 두 번째 인자(b)의 우선순위보다 큰지 비교한다.
  // 3. a가 더 크면 true를 반환하여, 큐 내부가 내림차순(높은 우선순위가 맨 앞)으로 정렬되도록 유지한다.
  return list_entry (a, struct thread, elem)->priority > list_entry (b, struct thread, elem)->priority;
}

void test_max_priority (void)
{
  // 1. 준비 큐(ready_list)가 비어있다면 비교할 대상이 없으므로 검사를 즉시 종료한다.
  if (list_empty (&ready_list))
    return;
    
  // 2. 준비 큐는 cmp_priority에 의해 항상 내림차순 정렬되어 있으므로, 큐의 맨 앞(front)에 있는 스레드가 가장 짱(우선순위가 가장 높음)이다.
  struct thread *highest = list_entry (list_front (&ready_list), struct thread, elem);
  
  // 3. 현재 CPU를 차지하고 있는 스레드의 우선순위와, 대기열 1등 스레드의 우선순위를 비교한다.
  if (thread_current ()->priority < highest->priority)
    {
      // 4. 대기 중인 스레드의 우선순위가 더 높다면 즉시 CPU를 양보해야 한다. (선점)
      if (intr_context ())
        // 외부 인터럽트(예: 타이머 인터럽트) 도중이라면, 인터럽트 처리가 끝날 때 문맥 교환이 일어나도록 예약(플래그 설정)한다.
        intr_yield_on_return ();
      else
        // 일반적인 코드 실행 흐름 중이라면, 즉각적으로 thread_yield()를 호출해 CPU를 넘겨준다.
        thread_yield ();
    }
}

// priority scheduling 여기 구현
void thread_aging (void)
{
  bool is_changed = false;
  struct list_elem *e;
  
  // 1. 준비 큐(ready_list)에 있는 모든 스레드를 순회한다.
  for (e = list_begin (&ready_list); e != list_end (&ready_list); e = list_next (e))
    {
      struct thread *t = list_entry (e, struct thread, elem);
      
      // 2. 스레드의 현재 우선순위가 최고값(PRI_MAX)보다 작다면 1 증가시킨다. (나이 먹기)
      if (t->priority < PRI_MAX) 
        {
          t->priority++;
          is_changed = true;
        }
    }
    
  // 3. 우선순위가 하나라도 변경되었다면 큐의 정렬 상태가 뒤틀렸을 수 있으므로 다시 내림차순 정렬한다.
  if (is_changed)
    list_sort (&ready_list, cmp_priority, NULL);
}

// advanced scheduler 여기 구현
// 1. 스레드의 우선순위를 새로 계산하는 함수
void mlfqs_calculate_priority (struct thread *t)
{
  if (t == idle_thread)
    return;
    
  // priority = PRI_MAX - (recent_cpu / 4) - (nice * 2)
  int term1 = DIV_MIX (t->recent_cpu, 4);
  int term2 = t->nice * 2;
  int new_pri = PRI_MAX - FP_TO_INT_NEAREST (term1) - term2;
  
  if (new_pri > PRI_MAX)
    t->priority = PRI_MAX;
  else if (new_pri < PRI_MIN)
    t->priority = PRI_MIN;
  else
    t->priority = new_pri;
}

// 2. 스레드의 recent_cpu를 새로 계산하는 함수
void mlfqs_calculate_recent_cpu (struct thread *t)
{
  if (t == idle_thread)
    return;
    
  // recent_cpu = (2 * load_avg) / (2 * load_avg + 1) * recent_cpu + nice
  int load_avg_2 = MUL_MIX (load_avg, 2);
  int load_avg_2_plus_1 = ADD_MIX (load_avg_2, 1);
  int coefficient = DIV_FP (load_avg_2, load_avg_2_plus_1);
  
  t->recent_cpu = ADD_MIX (MUL_FP (coefficient, t->recent_cpu), t->nice);
}

// 3. 시스템 전체의 load_avg를 새로 계산하는 함수
void mlfqs_calculate_load_avg (void)
{
  int ready_threads = list_size (&ready_list);
  if (thread_current () != idle_thread)
    ready_threads++;
    
  // load_avg = (59/60) * load_avg + (1/60) * ready_threads
  int term1 = MUL_FP (DIV_FP (INT_TO_FP (59), INT_TO_FP (60)), load_avg);
  int term2 = MUL_MIX (DIV_FP (INT_TO_FP (1), INT_TO_FP (60)), ready_threads);
  
  load_avg = ADD_FP (term1, term2);
}

// 4. 실행 중인 스레드의 recent_cpu를 1 증가시키는 함수
void mlfqs_increment_recent_cpu (void)
{
  if (thread_current () != idle_thread)
    thread_current ()->recent_cpu = ADD_MIX (thread_current ()->recent_cpu, 1);
}

// 5. 모든 스레드의 priority와 recent_cpu를 재계산하는 함수
void mlfqs_recalculate_recent_cpu (void)
{
  struct list_elem *e;
  
  for (e = list_begin (&all_list); e != list_end (&all_list); e = list_next (e))
    {
      struct thread *t = list_entry (e, struct thread, allelem);
      mlfqs_calculate_recent_cpu (t);
    }
}

void mlfqs_recalculate_priority (void)
{
  struct list_elem *e;
  
  for (e = list_begin (&all_list); e != list_end (&all_list); e = list_next (e))
    {
      struct thread *t = list_entry (e, struct thread, allelem);
      mlfqs_calculate_priority (t);
    }
    
  // ready_list에 있는 스레드들의 우선순위가 바뀌었을 수 있으므로 다시 정렬
  if (!list_empty (&ready_list))
    list_sort (&ready_list, cmp_priority, NULL);
}

/* Initializes the threading system by transforming the code
   that's currently running into a thread.  This can't work in
   general and it is possible in this case only because loader.S
   was careful to put the bottom of the stack at a page boundary.

   Also initializes the run queue and the tid lock.

   After calling this function, be sure to initialize the page
   allocator before trying to create any threads with
   thread_create().

   It is not safe to call thread_current() until this function
   finishes. */
void
thread_init (void) 
{
  ASSERT (intr_get_level () == INTR_OFF);

  lock_init (&tid_lock);
  list_init (&ready_list);
  list_init (&all_list);
  
  // alarm clock 여기 구현
  list_init (&sleeping_thread_list);

  // advanced scheduler 여기 구현
  load_avg = 0;

  /* Set up a thread structure for the running thread. */
  initial_thread = running_thread ();
  init_thread (initial_thread, "main", PRI_DEFAULT);
  initial_thread->status = THREAD_RUNNING;
  initial_thread->tid = allocate_tid ();
}

/* Starts preemptive thread scheduling by enabling interrupts.
   Also creates the idle thread. */
void
thread_start (void) 
{
  /* Create the idle thread. */
  struct semaphore idle_started;
  sema_init (&idle_started, 0);
  thread_create ("idle", PRI_MIN, idle, &idle_started);

  /* Start preemptive thread scheduling. */
  intr_enable ();

  /* Wait for the idle thread to initialize idle_thread. */
  sema_down (&idle_started);
}

/* Called by the timer interrupt handler at each timer tick.
   Thus, this function runs in an external interrupt context. */
void
thread_tick (void) 
{
  struct thread *t = thread_current ();

  /* Update statistics. */
  if (t == idle_thread)
    idle_ticks++;
#ifdef USERPROG
  else if (t->pagedir != NULL)
    user_ticks++;
#endif
  else
    kernel_ticks++;
    
  // priority scheduling & advanced scheduler 여기 구현
  if (thread_mlfqs)
    {
      // 1. 매 틱마다 현재 실행 중인 스레드의 recent_cpu를 1 증가시킨다.
      mlfqs_increment_recent_cpu ();
      
      // 2. 매 1초(TIMER_FREQ)마다 모든 스레드의 recent_cpu와 load_avg를 갱신한다.
      if (timer_ticks () % TIMER_FREQ == 0)
        {
          mlfqs_calculate_load_avg ();
          mlfqs_recalculate_recent_cpu ();
        }
        
      // 3. 매 4틱마다 모든 스레드의 priority를 재계산한다.
      if (timer_ticks () % TIME_SLICE == 0)
        mlfqs_recalculate_priority ();
    }
  else
    {
#ifndef USERPROG
      /* Project #3. */
      if (thread_prior_aging == true)
        thread_aging ();
#endif
    }

  /* Enforce preemption. */
  if (++thread_ticks >= TIME_SLICE)
    intr_yield_on_return ();
}

/* Prints thread statistics. */
void
thread_print_stats (void) 
{
  printf ("Thread: %lld idle ticks, %lld kernel ticks, %lld user ticks\n",
          idle_ticks, kernel_ticks, user_ticks);
}

/* Creates a new kernel thread named NAME with the given initial
   PRIORITY, which executes FUNCTION passing AUX as the argument,
   and adds it to the ready queue.  Returns the thread identifier
   for the new thread, or TID_ERROR if creation fails.

   If thread_start() has been called, then the new thread may be
   scheduled before thread_create() returns.  It could even exit
   before thread_create() returns.  Contrariwise, the original
   thread may run for any amount of time before the new thread is
   scheduled.  Use a semaphore or some other form of
   synchronization if you need to ensure ordering.

   The code provided sets the new thread's `priority' member to
   PRIORITY, but no actual priority scheduling is implemented.
   Priority scheduling is the goal of Problem 1-3. */
tid_t
thread_create (const char *name, int priority,
               thread_func *function, void *aux) 
{
  struct thread *t;
  struct kernel_thread_frame *kf;
  struct switch_entry_frame *ef;
  struct switch_threads_frame *sf;
  tid_t tid;

  ASSERT (function != NULL);

  /* Allocate thread. */
  t = palloc_get_page (PAL_ZERO);
  if (t == NULL)
    return TID_ERROR;

  /* Initialize thread. */
  init_thread (t, name, priority);
  tid = t->tid = allocate_tid ();
  
  // 시스템콜 여기 구현 - 새로 생성된 스레드(자식)에게 부모 스레드를 지정 (최초 스레드 제외)
#ifdef USERPROG
  if (t != initial_thread)
    t->parent = thread_current ();
#endif

  /* Stack frame for kernel_thread(). */
  kf = alloc_frame (t, sizeof *kf);
  kf->eip = NULL;
  kf->function = function;
  kf->aux = aux;

  /* Stack frame for switch_entry(). */
  ef = alloc_frame (t, sizeof *ef);
  ef->eip = (void (*) (void)) kernel_thread;

  /* Stack frame for switch_threads(). */
  sf = alloc_frame (t, sizeof *sf);
  sf->eip = switch_entry;
  sf->ebp = 0;

  /* Add to run queue. */
  thread_unblock (t);

  // priority scheduling 여기 구현
  test_max_priority ();

  return tid;
}

/* Puts the current thread to sleep.  It will not be scheduled
   again until awoken by thread_unblock().

   This function must be called with interrupts turned off.  It
   is usually a better idea to use one of the synchronization
   primitives in synch.h. */
void
thread_block (void) 
{
  ASSERT (!intr_context ());
  ASSERT (intr_get_level () == INTR_OFF);

  // priority scheduling 여기 구현
  // 1. 특정 자원을 기다리며 잠들기 때문에, 에이징된 우선순위를 원래(init_priority)대로 되돌린다.
  // (단, MLFQS 모드일 때는 우선순위를 건드리지 않는다.)
  if (!thread_mlfqs)
    thread_current ()->priority = thread_current ()->init_priority;

  thread_current ()->status = THREAD_BLOCKED;
  schedule ();
}

/* Transitions a blocked thread T to the ready-to-run state.
   This is an error if T is not blocked.  (Use thread_yield() to
   make the running thread ready.)

   This function does not preempt the running thread.  This can
   be important: if the caller had disabled interrupts itself,
   it may expect that it can atomically unblock a thread and
   update other data. */
void
thread_unblock (struct thread *t) 
{
  enum intr_level old_level;

  ASSERT (is_thread (t));

  old_level = intr_disable ();
  ASSERT (t->status == THREAD_BLOCKED);
  
  // priority scheduling 여기 구현
  // list_push_back (&ready_list, &t->elem);
  list_insert_ordered (&ready_list, &t->elem, cmp_priority, NULL);
  
  t->status = THREAD_READY;
  intr_set_level (old_level);
}

/* Returns the name of the running thread. */
const char *
thread_name (void) 
{
  return thread_current ()->name;
}

/* Returns the running thread.
   This is running_thread() plus a couple of sanity checks.
   See the big comment at the top of thread.h for details. */
struct thread *
thread_current (void) 
{
  struct thread *t = running_thread ();
  
  /* Make sure T is really a thread.
     If either of these assertions fire, then your thread may
     have overflowed its stack.  Each thread has less than 4 kB
     of stack, so a few big automatic arrays or moderate
     recursion can cause stack overflow. */
  ASSERT (is_thread (t));
  ASSERT (t->status == THREAD_RUNNING);

  return t;
}

/* Returns the running thread's tid. */
tid_t
thread_tid (void) 
{
  return thread_current ()->tid;
}

/* Deschedules the current thread and destroys it.  Never
   returns to the caller. */
void
thread_exit (void) 
{
  ASSERT (!intr_context ());

#ifdef USERPROG
  process_exit ();
#endif

  /* Remove thread from all threads list, set our status to dying,
     and schedule another process.  That process will destroy us
     when it calls thread_schedule_tail(). */
  intr_disable ();
  list_remove (&thread_current()->allelem);
  thread_current ()->status = THREAD_DYING;
  schedule ();
  NOT_REACHED ();
}

/* Yields the CPU.  The current thread is not put to sleep and
   may be scheduled again immediately at the scheduler's whim. */
void
thread_yield (void) 
{
  struct thread *cur = thread_current ();
  enum intr_level old_level;
  
  ASSERT (!intr_context ());

  old_level = intr_disable ();
  if (cur != idle_thread) 
    {
      // priority scheduling 여기 구현
      // 1. CPU를 양보하고 대기열로 돌아가므로, 에이징으로 부풀려진 우선순위를 원래(init_priority)대로 초기화한다.
      // (단, 고급 스케줄러(MLFQS) 모드일 때는 우선순위를 임의로 조작하지 않는다.)
      if (!thread_mlfqs)
        cur->priority = cur->init_priority;
      
      // 2. 초기화(또는 유지)된 우선순위에 맞게 준비 큐에 정렬 삽입한다.
      list_insert_ordered (&ready_list, &cur->elem, cmp_priority, NULL);
    }
  cur->status = THREAD_READY;
  schedule ();
  intr_set_level (old_level);
}

// alarm clock 여기 구현
/* Compares the wakeup_tick of two threads. */
static bool 
wakeup_tick_less (const struct list_elem *a,
                  const struct list_elem *b,
                  void *aux UNUSED) 
{
  const struct thread *ta = list_entry (a, struct thread, elem);
  const struct thread *tb = list_entry (b, struct thread, elem);
  return ta->wakeup_tick < tb->wakeup_tick;
}

void 
thread_sleep (int64_t ticks) 
{
  struct thread *cur = thread_current ();
  enum intr_level old_level;
  
  ASSERT (!intr_context ());
  
  // 1. 인터럽트를 비활성화하여 대기열을 조작하는 동안 꼬이지 않도록 보호한다. (임계 구역 진입)
  old_level = intr_disable ();
  
  // 2. 현재 스레드가 언제 깨어나야 하는지, 기상 시간(ticks)을 자신의 구조체에 기록한다.
  cur->wakeup_tick = ticks;
  
  // 3. 잠자는 스레드들의 대기실(sleeping_thread_list)에 현재 스레드를 넣는다. 
  //    이때 무조건 맨 뒤에 넣는 것이 아니라, 가장 일찍 깨어나야 할 스레드가 큐의 맨 앞에 오도록 
  //    wakeup_tick_less 비교 함수를 써서 오름차순으로 정렬 삽입한다.
  list_insert_ordered (&sleeping_thread_list, &cur->elem, wakeup_tick_less, NULL);
  
  // 4. 현재 스레드의 상태를 THREAD_BLOCKED로 멈춰두고 CPU를 반납하여 깊은 잠에 빠진다.
  thread_block ();
  
  // 5. 훗날 일어날 시간이 다 되어 thread_awake()에 의해 깨어나면 이 줄부터 실행되며, 이전의 인터럽트 상태를 원상 복구한다.
  intr_set_level (old_level);
}

void 
thread_awake (int64_t current_ticks) 
{
  // 1. 잠자는 스레드 큐(sleeping_thread_list)의 맨 앞 요소(가장 일찍 깰 스레드)부터 순서대로 검사를 시작한다.
  struct list_elem *e = list_begin (&sleeping_thread_list);
  
  while (e != list_end (&sleeping_thread_list)) 
    {
      struct thread *t = list_entry (e, struct thread, elem);
      
      // 2. 컴퓨터의 현재 시간(current_ticks)이 해당 스레드의 기상 시간(wakeup_tick)을 지났거나 같은지 확인한다.
      if (current_ticks >= t->wakeup_tick) 
        {
          // 3. 일어날 시간이 다 되었다면, 대기실 큐에서 해당 스레드를 쏙 빼낸다(remove). 
          //    그리고 thread_unblock()을 호출해 상태를 THREAD_READY로 바꾼 뒤 준비 큐로 올려보낸다.
          e = list_remove (e);
          thread_unblock (t);
        }
      else 
        {
          // 4. sleeping_thread_list는 이미 깰 시간이 빠른 순서대로 완벽하게 정렬되어 있다.
          //    따라서, 아직 깰 시간이 안 된 스레드를 단 한 번이라도 마주쳤다면, 
          //    그 뒤에 서 있는 스레드들은 볼 필요도 없이 깰 시간이 안 된 것이므로 검사를 즉시 멈추고 탈출(break)한다.
          break; 
        }
    }
    
  // 5. 스레드들을 쫙 깨워주고 난 뒤, 방금 깨어난 스레드 중 지금 실행 중인 나보다 우선순위가 높은 애가 있는지 확인하고 알아서 양보한다.
  // priority scheduling 여기 구현
  test_max_priority ();
}

/* Invoke function 'func' on all threads, passing along 'aux'.
   This function must be called with interrupts off. */
void
thread_foreach (thread_action_func *func, void *aux)
{
  struct list_elem *e;

  ASSERT (intr_get_level () == INTR_OFF);

  for (e = list_begin (&all_list); e != list_end (&all_list);
       e = list_next (e))
    {
      struct thread *t = list_entry (e, struct thread, allelem);
      func (t, aux);
    }
}

/* Sets the current thread's priority to NEW_PRIORITY. */
void
thread_set_priority (int new_priority) 
{
  if (thread_mlfqs) return;

  // priority scheduling 여기 구현
  // 1. 에이징을 위한 원래 우선순위(init_priority)와 현재 우선순위(priority)를 모두 새 값으로 변경한다.
  thread_current ()->init_priority = new_priority;
  thread_current ()->priority = new_priority;
  
  // 2. 우선순위가 바뀌었으므로 뺏길 상황인지 검사한다.
  test_max_priority ();
}

/* Returns the current thread's priority. */
int
thread_get_priority (void) 
{
  return thread_current ()->priority;
}

/* Sets the current thread's nice value to NICE. */
void
thread_set_nice (int nice) 
{
  // advanced scheduler 여기 구현
  // 1. 현재 스레드의 nice 값을 설정한다.
  thread_current ()->nice = nice;
  
  // 2. nice 값이 바뀌었으므로 자신의 우선순위를 다시 계산한다.
  mlfqs_calculate_priority (thread_current ());
  
  // 3. 우선순위가 떨어졌을 수 있으므로 다른 스레드에게 선점당해야 하는지 검사한다.
  test_max_priority ();
}

/* Returns the current thread's nice value. */
int
thread_get_nice (void) 
{
  // advanced scheduler 여기 구현
  return thread_current ()->nice;
}

/* Returns 100 times the system load average. */
int
thread_get_load_avg (void) 
{
  // advanced scheduler 여기 구현
  // load_avg는 고정 소수점이므로 정수로 변환하여 반환한다. 반올림을 수행한다.
  return FP_TO_INT_NEAREST (MUL_MIX (load_avg, 100));
}

/* Returns 100 times the current thread's recent_cpu value. */
int
thread_get_recent_cpu (void) 
{
  // advanced scheduler 여기 구현
  // recent_cpu도 고정 소수점이므로 정수로 변환하여 반환한다. 반올림을 수행한다.
  return FP_TO_INT_NEAREST (MUL_MIX (thread_current ()->recent_cpu, 100));
}

/* Idle thread.  Executes when no other thread is ready to run.

   The idle thread is initially put on the ready list by
   thread_start().  It will be scheduled once initially, at which
   point it initializes idle_thread, "up"s the semaphore passed
   to it to enable thread_start() to continue, and immediately
   blocks.  After that, the idle thread never appears in the
   ready list.  It is returned by next_thread_to_run() as a
   special case when the ready list is empty. */
static void
idle (void *idle_started_ UNUSED) 
{
  struct semaphore *idle_started = idle_started_;
  idle_thread = thread_current ();
  sema_up (idle_started);

  for (;;) 
    {
      /* Let someone else run. */
      intr_disable ();
      thread_block ();

      /* Re-enable interrupts and wait for the next one.

         The `sti' instruction disables interrupts until the
         completion of the next instruction, so these two
         instructions are executed atomically.  This atomicity is
         important; otherwise, an interrupt could be handled
         between re-enabling interrupts and waiting for the next
         one to occur, wasting as much as one clock tick worth of
         time.

         See [IA32-v2a] "HLT", [IA32-v2b] "STI", and [IA32-v3a]
         7.11.1 "HLT Instruction". */
      asm volatile ("sti; hlt" : : : "memory");
    }
}

/* Function used as the basis for a kernel thread. */
static void
kernel_thread (thread_func *function, void *aux) 
{
  ASSERT (function != NULL);

  intr_enable ();       /* The scheduler runs with interrupts off. */
  function (aux);       /* Execute the thread function. */
  thread_exit ();       /* If function() returns, kill the thread. */
}

/* Returns the running thread. */
struct thread *
running_thread (void) 
{
  uint32_t *esp;

  /* Copy the CPU's stack pointer into `esp', and then round that
     down to the start of a page.  Because `struct thread' is
     always at the beginning of a page and the stack pointer is
     somewhere in the middle, this locates the curent thread. */
  asm ("mov %%esp, %0" : "=g" (esp));
  return pg_round_down (esp);
}

/* Returns true if T appears to point to a valid thread. */
static bool
is_thread (struct thread *t)
{
  return t != NULL && t->magic == THREAD_MAGIC;
}

/* Does basic initialization of T as a blocked thread named
   NAME. */
static void
init_thread (struct thread *t, const char *name, int priority)
{
  enum intr_level old_level;

  ASSERT (t != NULL);
  ASSERT (PRI_MIN <= priority && priority <= PRI_MAX);
  ASSERT (name != NULL);

  memset (t, 0, sizeof *t);
  t->status = THREAD_BLOCKED;
  strlcpy (t->name, name, sizeof t->name);
  t->stack = (uint8_t *) t + PGSIZE;
  t->priority = priority;
  
  // priority scheduling 여기 구현
  // 1. 스레드가 처음 생성될 때 부여받은 순수 우선순위를 기억해둔다.
  t->init_priority = priority;

  // advanced scheduler 여기 구현
  // 1. 최초의 스레드(main)는 nice와 recent_cpu를 0으로 초기화한다.
  // 2. 그 외의 자식 스레드는 부모 스레드(현재 실행 중인 스레드)의 값을 물려받는다.
  if (t == initial_thread)
    {
      t->nice = 0;
      t->recent_cpu = 0;
    }
  else
    {
      t->nice = thread_current ()->nice;
      t->recent_cpu = thread_current ()->recent_cpu;
    }
  
  t->magic = THREAD_MAGIC;
#ifdef USERPROG
  t->exit_status = 0;
  // 3. 여기를 수정해야함
  t->next_fd = 2; // 0(STDIN), 1(STDOUT) 예약되어 있으므로 2번부터 할당
  int i;
  for (i = 0; i < 128; i++)
    t->fd_table[i] = NULL;
    
  // 시스템콜 여기 구현 - 자식 리스트 및 exec 대기용 세마포어 초기화
  list_init (&t->child_list);
  sema_init (&t->load_sema, 0);
  t->load_success = false;
#endif

  old_level = intr_disable ();
  list_push_back (&all_list, &t->allelem);
  intr_set_level (old_level);
}

/* Allocates a SIZE-byte frame at the top of thread T's stack and
   returns a pointer to the frame's base. */
static void *
alloc_frame (struct thread *t, size_t size) 
{
  /* Stack data is always allocated in word-size units. */
  ASSERT (is_thread (t));
  ASSERT (size % sizeof (uint32_t) == 0);

  t->stack -= size;
  return t->stack;
}

/* Chooses and returns the next thread to be scheduled.  Should
   return a thread from the run queue, unless the run queue is
   empty.  (If the running thread can continue running, then it
   will be in the run queue.)  If the run queue is empty, return
   idle_thread. */
static struct thread *
next_thread_to_run (void) 
{
  if (list_empty (&ready_list))
    return idle_thread;
  else
    return list_entry (list_pop_front (&ready_list), struct thread, elem);
}

/* Completes a thread switch by activating the new thread's page
   tables, and, if the previous thread is dying, destroying it.

   At this function's invocation, we just switched from thread
   PREV, the new thread is already running, and interrupts are
   still disabled.  This function is normally invoked by
   thread_schedule() as its final action before returning, but
   the first time a thread is scheduled it is called by
   switch_entry() (see switch.S).

   It's not safe to call printf() until the thread switch is
   complete.  In practice that means that printf()s should be
   added at the end of the function.

   After this function and its caller returns, the thread switch
   is complete. */
void
thread_schedule_tail (struct thread *prev)
{
  struct thread *cur = running_thread ();
  
  ASSERT (intr_get_level () == INTR_OFF);

  /* Mark us as running. */
  cur->status = THREAD_RUNNING;

  /* Start new time slice. */
  thread_ticks = 0;

#ifdef USERPROG
  /* Activate the new address space. */
  process_activate ();
#endif

  /* If the thread we switched from is dying, destroy its struct
     thread.  This must happen late so that thread_exit() doesn't
     pull out the rug under itself.  (We don't free
     initial_thread because its memory was not obtained via
     palloc().) */
  if (prev != NULL && prev->status == THREAD_DYING && prev != initial_thread) 
    {
      ASSERT (prev != cur);
      palloc_free_page (prev);
    }
}

/* Schedules a new process.  At entry, interrupts must be off and
   the running process's state must have been changed from
   running to some other state.  This function finds another
   thread to run and switches to it.

   It's not safe to call printf() until thread_schedule_tail()
   has completed. */
static void
schedule (void) 
{
  struct thread *cur = running_thread ();
  struct thread *next = next_thread_to_run ();
  struct thread *prev = NULL;

  ASSERT (intr_get_level () == INTR_OFF);
  ASSERT (cur->status != THREAD_RUNNING);
  ASSERT (is_thread (next));

  if (cur != next)
    prev = switch_threads (cur, next);
  thread_schedule_tail (prev);
}

/* Returns a tid to use for a new thread. */
static tid_t
allocate_tid (void) 
{
  static tid_t next_tid = 1;
  tid_t tid;

  lock_acquire (&tid_lock);
  tid = next_tid++;
  lock_release (&tid_lock);

  return tid;
}

/* Offset of `stack' member within `struct thread'.
   Used by switch.S, which can't figure it out on its own. */
uint32_t thread_stack_ofs = offsetof (struct thread, stack);
