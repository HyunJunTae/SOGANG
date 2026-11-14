import numpy as np

class GoldenRectangle:
    def __init__(self, n):
        self.fibonacci_list = self.fibo(n)

    @staticmethod
    def fibo(n):
        fibo_list = [1, 1]

        for i in range(2, n) :
            number = fibo_list[i-2] + fibo_list[i-1]
            fibo_list.append(number)
        
        return fibo_list
    
    @staticmethod
    def right_operation(grid_cells, cur_rect_length, height, width):
        # TODO: Implement the right operation to fill the grid

        grid_cells[:, width - cur_rect_length:width] = cur_rect_length
        return grid_cells

    @staticmethod
    def down_operation(grid_cells, cur_rect_length, height, width):
        # TODO: Implement the down operation to fill the grid

        grid_cells[height - cur_rect_length : , : ] = cur_rect_length
        return grid_cells
    
    @staticmethod
    def left_operation(grid_cells, cur_rect_length, height, width):
        # TODO: Implement the left operation to fill the grid

        grid_cells[ : , 0 : cur_rect_length] = cur_rect_length
        return grid_cells

    @staticmethod
    def up_operation(grid_cells, cur_rect_length, height, width):
        # TODO: Implement the up operation to fill the grid

        grid_cells[0 : cur_rect_length , : ] = cur_rect_length
        return grid_cells

    # Do not change this function name (get_golden_rectangle)
    def get_golden_rectangle(self):
        # TODO: Implement the logic to create the golden rectangle grid

        n = len(self.fibonacci_list)
        height, width = self.fibonacci_list[-1], self.fibonacci_list[-1] + self.fibonacci_list[-2]  # 맨 뒤의 요소가 높이, 뒤에서 2번째 요소가 너비
        grid_cells = np.zeros((height, width)) # grid 만들어두기
        step=0

        # 큰 거부터 채워넣기.
        # 큰 거 채워넣고, sub grid 만들어서 그걸 전달.
        # i % 4. 나머지가 0 1 2 3 순으로 right down up left.

        sub_grid = grid_cells[:, :]
        for i in range(n-1, -1, -1) :

            height, width = sub_grid.shape
            cur_rect_length = self.fibonacci_list[i]

            # right
            if (step%4 == 2) :
                self.right_operation(sub_grid, cur_rect_length, height, width)
                sub_grid = sub_grid[ : , 0 : width - cur_rect_length]

            # down
            elif (step%4 == 3) :
                self.down_operation(sub_grid, cur_rect_length, height, width)
                sub_grid = sub_grid[0 : height - cur_rect_length , : ]

            # left
            elif (step%4 == 0) :
                self.left_operation(sub_grid, cur_rect_length, height, width)
                sub_grid = sub_grid[ : , cur_rect_length : ]

            # up
            elif (step%4 == 1) :
                self.up_operation(sub_grid, cur_rect_length, height, width)
                sub_grid = sub_grid[ cur_rect_length : , : ]
            
            step += 1
        
        
        # Return the grid cells as an (H, W) ndarray
        return grid_cells
        
        
# Instructions: Complete only the class implementation. You may add print statements to test when using the "Run" button, 
# but only the class will be used for grading upon submission.

# n = 7
# golden_rectangle_instance = GoldenRectangle(n)
# golden_rectangle = golden_rectangle_instance.get_golden_rectangle()
# print(golden_rectangle)

# Explanation on the difference with @staticmethod:
# Using @staticmethod:
#   - Methods marked with @staticmethod do not need access to any instance-specific data, 
#     so they can be called directly from the class, e.g., GoldenRectangle.fibo(10).
#   - They do not access self or instance variables.
# Without @staticmethod:
#   - Methods without @staticmethod need an instance of the class to access instance-specific data (like self.fibonacci_list).
#   - They are called through an instance, e.g., golden_rectangle_instance.get_golden_rectangle().

