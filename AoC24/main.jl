using DataStructures

function print_map(map::Array{Bool, 2})
    for i in 1:size(map)[2]
        for j in 1:size(map)[1]
            if map[j, i]
                print('\u2588')
            else
                print('\u2591')
            end
        end
        print('\n')
    end
end

function load_map(input_file)
    file = split(read(input_file, String), "\r\n")
    file = filter!(x-> length(x) > 0, file)

    mapsize = 5
    map = Array{Bool, 2}(undef, (mapsize, mapsize))
    for i in 1:mapsize
        for j in 1:mapsize
            map[j, i] = (file[i][j] == '#')
        end
    end
    return map
end

function biodiversity_rating(map::Array{Bool, 2})
    result = 0
    current_multiplier = 1
    for i in 1:length(map)
        if map[i]
            result += current_multiplier
        end
        current_multiplier *= 2
    end
    return result
end

function t1()
    map = load_map("C:\\Projects\\AdventOfCode\\Solutions\\AoC24\\input.txt")
    next_step = Array{Bool, 2}(undef, size(map))

    function next_gen(current::Array{Bool, 2}, next::Array{Bool, 2})
        dims = size(current)
        dx = [0, 1, 0, -1]
        dy = [-1, 0, 1, 0]

        function neighbours(position::Tuple{Int64, Int64})
            total = 0
            for i in 1:4
                new_position = position[1] + dx[i], position[2] + dy[i]
                if  1 <= new_position[1] <= dims[1] &&
                    1 <= new_position[2] <= dims[2]
                    if current[new_position[1], new_position[2]]
                        total += 1
                    end
                end
            end
            return total
        end

        for i in 1:dims[2]
            for j in 1:dims[1]
                if current[j, i]
                    next[j, i] = (neighbours((j, i)) == 1)
                else
                    next[j, i] = (1 <= neighbours((j, i)) <= 2)
                end
            end
        end
    end

    previous_layouts = Array{Array{Bool, 2}, 1}(undef, 0)
    push!(previous_layouts, deepcopy(map))

    while true
        next_gen(map, next_step)
        map, next_step = next_step, map
        if in(map, previous_layouts)
            break
        end
        push!(previous_layouts, deepcopy(map))
    end

    println("First = $(biodiversity_rating(map))")
end

function t2()
    map = load_map("C:\\Projects\\AdventOfCode\\Solutions\\AoC24\\input.txt")

    map =       Dict{Int64, Array{Bool, 2}}([(0, map)])
    next_step = Dict{Int64, Array{Bool, 2}}([(0, fill(false, (5, 5)))])

    function next_gen(current::Dict{Int64, Array{Bool, 2}}, next::Dict{Int64, Array{Bool, 2}})
        max_depth, min_depth = 0, 0
        for m in map
            min_depth = min(m.first, min_depth)
            max_depth = max(m.first, max_depth)
        end
        current[min_depth - 1] = fill(false, (5, 5))
        current[max_depth + 1] = fill(false, (5, 5))
        next[min_depth - 1] = fill(false, (5, 5))
        next[max_depth + 1] = fill(false, (5, 5))
        min_depth -= 1
        max_depth += 1

        function neighbours(position::Tuple{Int64, Int64}, depth::Int64)
            total_neigbours = 0
            dx = [0, 1, 0, -1]
            dy = [-1, 0, 1, 0]

            starters = [(1, 5), (1, 1), (1, 1), (5, 1)]
            dirs = [(1, 0), (0, 1), (1, 0), (0, 1)]

            for direction in 1:4
                new_position = position[1] + dx[direction], position[2] + dy[direction]

                if new_position == (3, 3)
                    if depth != max_depth
                        p = [starters[direction][1], starters[direction][2]]
                        d = dirs[direction]
                        for i in 1:5
                            if current[depth + 1][p[1], p[2]]
                                total_neigbours += 1
                            end
                            p[1] += d[1]
                            p[2] += d[2]
                        end
                    end
                elseif new_position[1] == 0
                    if depth != min_depth
                        if current[depth - 1][12]
                            total_neigbours += 1
                        end
                    end
                elseif new_position[2] == 0
                    if depth != min_depth
                        if current[depth - 1][8]
                            total_neigbours += 1
                        end
                    end
                elseif new_position[1] == 6
                    if depth != min_depth
                        if current[depth - 1][14]
                            total_neigbours += 1
                        end
                    end
                elseif new_position[2] == 6
                    if depth != min_depth
                        if current[depth - 1][18]
                            total_neigbours += 1
                        end
                    end
                else
                    if current[depth][new_position[1], new_position[2]]
                        total_neigbours += 1
                    end
                end
            end
            return total_neigbours
        end

        for depth in min_depth:max_depth
            for y in 1:5
                for x in 1:5
                    if x == y == 3 continue end
                    n = neighbours((x, y), depth)
                    if current[depth][x, y]
                        next[depth][x, y] = (n == 1)
                    else
                        next[depth][x, y] = (1 <= n <= 2)
                    end
                end
            end
        end
    end

    for gen in 1:200
        next_gen(map, next_step)
        map, next_step = next_step, map
    end

    total = 0
    for m in map
        for i in 1:25
            if m.second[i]
                total += 1
            end
        end
    end

    println("Second = $total")
end

"""
First = 30442557
Second = 1987
"""


t1()
t2()
