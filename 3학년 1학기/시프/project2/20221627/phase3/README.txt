[system programming lecture]

-project 2 baseline

csapp.{c,h}
        CS:APP3e functions

myshell.c
        함수 추가
        void initjobs();
        jobs 장부를 초기화

        int addjob(pid_t pid, int state, char *cmdline);
        jobs에 새로운 프로세스를 추가

        int deletejob(pid_t pid);
        jobs에서 프로세스를 제거

        void clearjob(struct job_t *job);
        jobs에서 프로세스를 제거할 때, struct의 데이터를 모두 0으로 만들어줌.

        struct job_t *getjobpid(pid_t pid);
        특정 프로세스의 pid를 가져오기

        struct job_t *getjobjid(int jid);
        특정 프로세스의 jid를 가져오기

        void listjobs();
        각 프로세스가 어떤 상태(running / foreground / stopped / unknown) 인지 출력



        void sigchld_handler(int sig);
        자식 프로세스가 종료되거나 멈췄을 때 처리해주기

        void sigtstp_handler(int sig);
        컨트롤 C 처리. 현재 포그라운드에서 실행 중인 모든 프로세스에게 SIGINT 시그널 보내서 프로세스 종료시키기.

        void sigint_handler(int sig);
        컨트롤 Z 처리. 현재 포그라운드에서 실행 중인 모든 프로세스에게 SIGTSTP 시그널 보내서 STOPPED STATE로 만들기.
        SIGCONT 시그널을 받을 때까지 STOP.

        pid_t fgpid(struct job_t *jobs);
        현재 포그라운드에서 실행 중인 작업의 PID를 반환 (없으면 0)

        int pid2jid(pid_t pid);
        PID를 이용해 해당 작업의 JID를 반환

        void waitfg(pid_t pid);
        포그라운드 작업이 더 이상 FG 상태가 아닐 때까지(종료되거나 정지될 때까지) 대기

        void do_bgfg_kill(char **argv);
        BG, FG, KILL 명령어 처리

        void reap_terminated_jobs(struct job_t *jobs);
        TM 상태의 job 들을 처리

        int get_next_jid(struct job_t *jobs);
        현재 장부에 있는 작업 중 가장 큰 JID를 찾아 +1을 반환

        함수 수정
        eval - parse_pipe를 추가하고, 파이프 개수에 따라 실행 방식 나누기
        
myshell.h

        구조체 추가
        job_t

        함수 추가
        void initjobs(); 
        int addjob(pid_t pid, int state, char *cmdline);
        int deletejob(pid_t pid);
        void clearjob(struct job_t *job);
        struct job_t *getjobpid(pid_t pid);
        struct job_t *getjobjid(int jid); 
        void listjobs();

        void sigchld_handler(int sig);
        void sigtstp_handler(int sig);
        void sigint_handler(int sig);
        pid_t fgpid(struct job_t *jobs);
        int pid2jid(pid_t pid);
        void waitfg(pid_t pid);
        void do_bgfg_kill(char **argv);
        void reap_terminated_jobs(struct job_t *jobs);
        int get_next_jid(struct job_t *jobs);