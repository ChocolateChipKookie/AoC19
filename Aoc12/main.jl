function load_planets()
    file = open("C:\\Projects\\AdventOfCode\\Solutions\\AoC12\\input.txt") do file
        read(file, String)
    end
    file = split(file, "\r\n")

    planets = Array{Array{Int64, 1}, 1}(undef, 0)

    for line in file
        if line == ""
            continue
        end
        line = line[2:end - 1]
        substrings = split(line, ", ")
        p = Array{Int64, 1}([
            parse(Int64, substrings[1][3:end]),
            parse(Int64, substrings[2][3:end]),
            parse(Int64, substrings[3][3:end]),
            0, 0, 0])
        push!(planets, p)
    end

    return planets
end

function t1()
    planets = load_planets()

    for _ in 1:1000
        for i in 1:length(planets)
            for j in i + 1: length(planets)
                for dim in 1:3
                    if planets[i][dim] < planets[j][dim]
                        planets[i][dim+3] += 1
                        planets[j][dim+3] -= 1
                    elseif planets[i][dim] > planets[j][dim]
                        planets[i][dim+3] -= 1
                        planets[j][dim+3] += 1
                    end
                end
            end
        end

        for planet in planets
            for dim in 1:3
                planet[dim] += planet[dim+3]
            end
        end
    end

    energy = 0
    for planet in planets
        pot, kin = 0, 0
        for dim in 1:3
            pot += abs(planet[dim])
            kin += abs(planet[dim + 3])
        end
        energy += pot * kin
    end
    println("First = $energy")
end

function t2()
    planets = load_planets()

    #Recurring pattern for all dimensions
    recurring = fill(0, 3)
    for axis in 1:3
        #Initial state
        init = [x[axis] for x = planets]
        #Don't know why, but it works
        steps = 2

        while true
            #Gravity update
            for i in 1:length(planets)
                for j in i + 1: length(planets)
                    if planets[i][axis] < planets[j][axis]
                        planets[i][axis+3] += 1
                        planets[j][axis+3] -= 1
                    elseif planets[i][axis] > planets[j][axis]
                        planets[i][axis+3] -= 1
                        planets[j][axis+3] += 1
                    end
                end
            end

            #Update
            for planet in planets
                planet[axis] += planet[axis+3]
            end
            #Exit condition
            if [x[axis] for x = planets] == init
                break
            end
            #Update steps
            steps += 1
        end
        recurring[axis] = steps
    end
    #Least common multiple of the steps for all dimensions
    println("Second = $(lcm(recurring))")
end

#First = 7758
#Second = 354540398381256

t1()
t2()
