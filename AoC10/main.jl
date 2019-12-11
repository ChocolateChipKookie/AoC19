using DataStructures

width, height = 0, 0

#Loading the input
function load()
    file = open("C:\\Projects\\AdventOfCode\\Solutions\\AoC10\\input.txt") do file
        read(file, String)
    end

    file = split(file, '\n')
    global height = length(file)
    global width = length(file[1])
    asteroids = []
    x , y = 0, 0

    for line in file
        for character in line
            if character == '#'
                push!(asteroids, (x, y))
            end
            x += 1
        end
        x = 0
        y += 1
    end
    return asteroids
end

#Calculating the angle in their way
function angle(point1, point2)
    #dx, dy = point2[1] - point1[1], point2[2] - point1[2]
    #return mod(atan(dy, dx) + pi/2, 2pi)
    #Get the atan2 angle, shift it by 90
    return mod(atan(point2[2] - point1[2], point2[1] - point1[1]) + pi/2, 2pi)
end

#First task
function f1(asteroids)
    #Result variables
    top = 0
    result = Nothing
    #Test for every asteroid
    for asteroid in asteroids
        #For unique elements
        counter = Set()
        for other in asteroids
            if other != asteroid
                #Add the angle to the set
                push!(counter, angle(asteroid, other))
            end
        end
        if length(counter) > top
            top = length(counter)
            result = asteroid
        end
    end
    return top, result
end

#Second task
function f2(asteroids, pos::Int64)
    asteroid_dict = DataStructures.SortedDict{Float64, Array{Any, 1}}()
    base = f1(asteroids)[2]

    #Fill dictionary
    for other in asteroids
        if other != base
            a = angle(base, other)
            if !haskey(asteroid_dict, a); asteroid_dict[a] = []; end
            push!(asteroid_dict[a], other)
        end
    end

    #Sort the lines of sight by distance to the base
    foreach(asteroid_line -> sort!(asteroid_line.second, by= x-> (x[1] - base[1])^2 + (x[2] - base[2])^2), asteroid_dict)

    index = 1
    while true
        for asteroid_line in asteroid_dict
            #If there is no more asteroids at that angle, ignore
            if length(asteroid_line) == 0
                continue
            end

            #Fetch asteroid
            asteroid = asteroid_line.second[1]
            if index == pos
                return asteroid
            end

            #Increment and delete asteroid
            index += 1
            deleteat!(asteroid_line.second, 1)
        end
    end
end

"""
First = 284
Second = At position (4, 4), with value 404
"""

asteroids = load()
println("First = $(f1(asteroids)[1])")
ans = f2(asteroids, 200)
println("Second = At position $ans, with value $(100* ans[1] + ans[2])")
