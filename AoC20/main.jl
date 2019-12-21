using DataStructures

function load_maze(path)
    file = open(path) do file
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

function print_maze(maze)
    for line in maze
        println(join(line, ' '))
    end
end

mutable struct State
    pos::Tuple{Int64, Int64}
    path_length::Int64
end

function t1()
    maze, width, height = load_maze("C:\\Projects\\AdventOfCode\\Solutions\\AoC20\\input.txt")

    start::Tuple{Int64, Int64}  = (0, 0)
    finish::Tuple{Int64, Int64} = (0, 0)

    dx = [0, -1, 0, 1]
    dy = [-1, 0, 1, 0]

    portals = Dict{Tuple{Int64, Int64}, Tuple{Int64, Int64}}()

    #Sorted 2 characters, position of the mid char, position of the point
    warps = Dict{String, Tuple{Tuple{Int64, Int64}, Tuple{Int64, Int64}}}()

    for i in 2: height - 1, j in 2:width - 1
        if 'A' <= maze[i][j] <= 'Z'
            entry = maze[i][j]
            other = '0'
            point_pos = (0, 0)

            for dir in 1:4
                point_pos = j+ dx[dir], i + dy[dir]
                val = maze[point_pos[2]][point_pos[1]]
                if val == '.'
                    opposite_dir = mod1(dir + 2, 4)
                    other = maze[i + dy[opposite_dir]][j+ dx[opposite_dir]]
                    break
                end
            end

            if other != '0'
                if other == entry == 'A'
                    start = point_pos
                    maze[i][j] = '#'
                    continue
                elseif other == entry == 'Z'
                    finish = point_pos
                    maze[i][j] = '#'
                    continue
                else
                    tmp = [entry, other]
                    sort!(tmp)
                    key = join(tmp)
                    if in(key, keys(warps))
                        tmp = warps[key]
                        maze[i][j] = '@'
                        maze[tmp[1][2]][tmp[1][1]] = '@'

                        portals[tmp[1]] = point_pos
                        portals[(j, i)] = tmp[2]
                    else
                        warps[key] = (j, i), point_pos
                    end
                end
            end
        end
    end

    visited_states = Set{Tuple{Int64, Int64}}([start])
    to_visit = PriorityQueue{State, Int64}([(State(start, 0), 0)])

    while !isempty(to_visit)
        current_state = dequeue!(to_visit)

        if current_state.pos == finish
            println("First = $(current_state.path_length)")
        end

        for direction in 1:4
            new_pos = (current_state.pos[1] + dx[direction], current_state.pos[2] + dy[direction])

            pos_value = maze[new_pos[2]][new_pos[1]]
            if pos_value == '#'
                continue
            elseif pos_value == '@'
                new_pos = portals[new_pos]
            end

            if in(new_pos, visited_states)
                continue
            end

            to_visit[State(new_pos, current_state.path_length + 1)] = current_state.path_length + 1
            push!(visited_states, new_pos)
        end
    end
end

function t2()

end

"""
First: 73127523
Second: 80284420
"""

t1()
t2()
