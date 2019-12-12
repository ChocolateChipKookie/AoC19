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
                p1 = planets[i]
                p2 = planets[j]

                for dim in 1:3
                    if p1[dim] < p2[dim]
                        p1[dim+3] += 1
                        p2[dim+3] -= 1
                    elseif p1[dim] > p2[dim]
                        p1[dim+3] -= 1
                        p2[dim+3] += 1
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
        pot = 0
        kin = 0
        for dim in 1:3
            pot += abs(planet[dim])
            kin += abs(planet[dim + 3])
        end
        total = pot * kin
        energy += total
    end
    println("First = $energy")
end

function t2()
    planets = load_planets()
    steps = 1

    recurring = fill(0, 3)

    for axis in 1:3
        init = Array{Int64, 1}(undef, 0)


        for planet in planets
            push!(init, planet[axis])
        end

        current = Array{Int64, 1}(undef, length(planets))
        steps = 2

        while true
            for i in 1:length(planets)
                for j in i + 1: length(planets)
                    p1 = planets[i]
                    p2 = planets[j]
                    if p1[axis] < p2[axis]
                        p1[axis+3] += 1
                        p2[axis+3] -= 1
                    elseif p1[axis] > p2[axis]
                        p1[axis+3] -= 1
                        p2[axis+3] += 1
                    end
                end
            end

            for planet in planets
                planet[axis] += planet[axis+3]
            end

            for index in 1:length(planets)
                current[index] = planets[index][axis]
            end


            if current == init
                break
            end

            steps += 1
        end

        recurring[axis] = steps
    end

    tmp = lcm(recurring[1], recurring[2], recurring[3])
    println("Second = $tmp")
end

#First = 7758
#Second = 354540398381256

t1()
t2()
