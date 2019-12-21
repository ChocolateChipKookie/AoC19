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

mutable struct State
    position::Tuple{Int64, Int64}
    total_steps::Int64
    keys::Set{Char}
end

function t1()
    maze, width, height = load_maze("C:\\Projects\\AdventOfCode\\Solutions\\AoC18\\input.txt")
    start_point::Tuple{Int64, Int64} = 0, 0

    dx = [0, -1, 0, 1]
    dy = [-1, 0, 1, 0]

    for i in 1:height, j in 1:width
        if maze[i][j] == '@'
            start_point = (j, i)
            maze[i][j] = '.'
            break
        end
    end

    visited_states = Set{Tuple{Tuple{Int64, Int64}, Set{Char}}}()
    to_visit = PriorityQueue{State, Int64}()
    to_visit[State(start_point, 0, Set{Char}())] = 0

    total_keys = 0

    for line in maze
        total_keys += count(x-> 'a' <= x <= 'z', line)
    end

    while !isempty(to_visit)
        current_state = dequeue!(to_visit)
        pos_value = maze[current_state.position[2]][current_state.position[1]]
        if in((current_state.position, current_state.keys), visited_states)
            continue
        end

        if 'A' <= pos_value <= 'Z'
            if !in(pos_value + 32, current_state.keys)
                continue
            end
        elseif 'a' <= pos_value <= 'z'
            push!(current_state.keys, pos_value)
            if length(current_state.keys) == total_keys
                println("First = $(current_state.total_steps)")
                break
            end
        end

        push!(visited_states, (current_state.position, current_state.keys))

        for i in 1:4
            next_position = (current_state.position[1] + dx[i], current_state.position[2] + dy[i])
            if maze[next_position[2]][next_position[1]] != '#'
                next_state = State(next_position, current_state.total_steps + 1, deepcopy(current_state.keys))
                to_visit[next_state] = current_state.total_steps + 1
            end
        end
    end
end

function t2()
    maze, width, height = load_maze("C:\\Projects\\AdventOfCode\\Solutions\\AoC18\\input.txt")
    start_points = Array{Tuple{Int64, Int64}}(undef, 0)

    dx = [0, -1, 0, 1]
    dy = [-1, 0, 1, 0]

    robots = Array{State, 1}(undef, 0)

    function collect_keys(x_range, y_range)
        keys = Set{Char}()
        for i in y_range[1]:y_range[2], j in x_range[1]:x_range[2]
            if 'a' <= maze[i][j] <= 'z'
                push!(keys, maze[i][j])
            end
        end
        return keys
    end

    quadrants = Array{State, 1}(undef, 4)

    all_keys = collect_keys((1, width), (1, height))
    total_keys = length(all_keys)
    for i in 1:height, j in 1:width
        if maze[i][j] == '@'
            maze[i][j] = '#'

            for k in 1:4
                maze[i + dy[k]][j + dx[k]] = '#'
            end
            quadrants[1] = State((j - 1, i - 1), 0, setdiff(all_keys, collect_keys((1, j - 1), (1, i - 1))))
            quadrants[2] = State((j + 1, i - 1), 0, setdiff(all_keys, collect_keys((j + 1, width), (1, i - 1))))
            quadrants[3] = State((j - 1, i + 1), 0, setdiff(all_keys, collect_keys((1, j - 1), (i + 1, height))))
            quadrants[4] = State((j + 1, i + 1), 0, setdiff(all_keys, collect_keys((j + 1, width), (i + 1, height))))
        end
    end

    results = Array{Int64, 1}(undef, 4)
    q = 1
    for quadrant in quadrants
        to_visit = PriorityQueue{State, Int64}()
        to_visit[quadrant] = 0
        visited_states = Set{Tuple{Tuple{Int64, Int64}, Set{Char}}}()

        while !isempty(to_visit)
            current_state = dequeue!(to_visit)
            pos_value = maze[current_state.position[2]][current_state.position[1]]

            if 'a' <= pos_value <= 'z'
                push!(current_state.keys, pos_value)
                if length(current_state.keys) == total_keys
                    results[q] = current_state.total_steps
                    q += 1
                    break
                end
            end

            for i in 1:4
                next_position = (current_state.position[1] + dx[i], current_state.position[2] + dy[i])

                if in((next_position, current_state.keys), visited_states)
                    continue
                end

                if 'A' <= pos_value <= 'Z'
                    if !in(pos_value + 32, current_state.keys)
                        continue
                    end
                end

                if maze[next_position[2]][next_position[1]] != '#'
                    next_state = State(next_position, current_state.total_steps + 1, deepcopy(current_state.keys))
                    push!(visited_states, (next_position, deepcopy(current_state.keys)));
                    to_visit[next_state] = current_state.total_steps + 1
                end
            end
        end
    end

    println("Second = $(sum(results))")
end

#I don't have any patience explaining what I did, as this was the most stressful experience of my life

"""
First = 5198
Second = 1736
"""

t1()
t2()
