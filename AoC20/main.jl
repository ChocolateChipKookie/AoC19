function load_maze()
    file = open("C:\\Projects\\AdventOfCode\\Solutions\\AoC20\\input.txt") do file
        read(file, String)
    end

    file = split(file, "\r\n")
    file = filter!(x-> length(x) > 0, file)
    width = length(file[1])
    height = length(file)

    maze = []

    for y in 1:height
        push!(maze, collect(file[y]))
    end

    return maze, width, height
end

function t1()

    maze, width, height = load_maze()

    start = (0, 0)

    for i in 2:height
        for j in 2:width
            
        end
    end




    print(maze)



end

function t2()
    #Reading file
    file = open("C:\\Projects\\AdventOfCode\\Solutions\\AoC20\\input.txt") do file
        read(file, String)
    end

end

"""
First: 73127523
Second: 80284420
"""

t1()
#t2()
