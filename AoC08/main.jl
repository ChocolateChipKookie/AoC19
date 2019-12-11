# Open and parse file
file = open("C:\\Projects\\AdventOfCode\\Solutions\\AoC08\\input.txt") do file
    read(file, String)
end

width, height = 25, 6
size = width * height
layers = div(length(file) , size)

function task1()

    result = 0
    min_zeros = size

    for i in 0:layers-1
        c = count(x->x=='0', file[i*size + 1: i*size + size])
        if c < min_zeros
            min_zeros = c
            result = count(x->x=='1', file[i*size + 1: i*size + size]) * count(x->x=='2', file[i*size + 1: i*size + size])
        end
    end
    println("First = $result")
end

function task2()

    result = fill('x', (width, height))
    for i in length(file):-1:1
        if file[i] != '2'
            result[mod1(mod1(i, size), width), cld(mod1(i, size), width)] = file[i]
        end
    end

    function printarr()
        for y in 1:height
            for x in 1:width
                if result[x, y] == '0'
                    print(' ')
                elseif result[x, y] == '1'
                    print('#')
                end
            end
            print('\n')
        end
    end

    println("Second:")
    printarr()
end
"""
First = 2016
Second:
#  # ####  ##  #### #  #
#  #    # #  #    # #  #
####   #  #      #  #  #
#  #  #   #     #   #  #
#  # #    #  # #    #  #
#  # ####  ##  ####  ##
"""
task1()
task2()
