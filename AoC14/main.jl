



file = open("C:\\Projects\\AdventOfCode\\Solutions\\AoC14\\input.txt") do file
    read(file, String)
end
file = split(file, "\r\n")

mutable struct chemical
    name::String
    amount::Int64
end

mutable struct reaction
    ingredients::Array{chemical, 1}
    result::chemical
end



reactions_dict = Dict{String, Any}()
reactions = Array{reaction, 1}(undef, 0)

for line in file
    if line == ""
        continue
    end
    substrings = split(line, "=>")

    ingredients = split(strip(substrings[1]), ", ")

    res = strip(substrings[2])
    res = split(res, " ")
    res = chemical(res[2], parse(Int64, res[1]))

    ingredients_ = Array{chemical, 1}(undef, 0)

    for substr in ingredients
        ing = strip(substr)
        ing = split(ing, " ")
        ing = chemical(ing[2], parse(Int64, ing[1]))
        push!(ingredients_, ing)
    end

    re = reaction(ingredients_, res)

    if !haskey(reactions_dict, res.name)
        reactions_dict[res.name] = (res.amount, ingredients_)
    end

    push!(reactions, re)
end

function t1()
    needed = Dict{String, Int64}()
    needed["FUEL"] = 1
    needed["ORE"] = 0
    available = Dict{String, Int64}()

    while !(length(needed) == 1)
        new_needed = Dict{String, Int64}()
        new_needed["ORE"] = needed["ORE"]
        for element in needed
            if element.first == "ORE"
                continue
            end

            if !haskey(available, element.first)
                available[element.first] = 0
            end

            if available[element.first] < element.second
                to_make = element.second - available[element.first]

                multiplier = cld(to_make, reactions_dict[element.first][1])

                available[element.first] += multiplier * reactions_dict[element.first][1]
                for chem in reactions_dict[element.first][2]
                    if !haskey(new_needed, chem.name)
                        new_needed[chem.name] = 0
                    end
                    new_needed[chem.name] += multiplier* chem.amount
                end
            end

            available[element.first] -= element.second
        end

        needed = new_needed
    end

    println("First :", needed["ORE"])

end

function t2()
    available = Dict{String, Int64}()

    total_ore = 1000000000000

    function create_fuel(a)
        needed = Dict{String, Int64}()
        needed["FUEL"] = a
        needed["ORE"] = 0
        while !(length(needed) == 1)
            new_needed = Dict{String, Int64}()
            new_needed["ORE"] = needed["ORE"]
            for element in needed
                if element.first == "ORE"
                    continue
                end

                if !haskey(available, element.first)
                    available[element.first] = 0
                end

                if available[element.first] < element.second
                    to_make = element.second - available[element.first]

                    multiplier = cld(to_make, reactions_dict[element.first][1])

                    available[element.first] += multiplier * reactions_dict[element.first][1]
                    for chem in reactions_dict[element.first][2]
                        if !haskey(new_needed, chem.name)
                            new_needed[chem.name] = 0
                        end
                        new_needed[chem.name] += multiplier* chem.amount
                    end
                end

                available[element.first] -= element.second
            end

            needed = new_needed
        end
        return needed["ORE"]
    end

    min_fuel = 0
    max_fuel = 1000000000000
    mid_fuel = div(min_fuel + max_fuel, 2)

    while min_fuel < max_fuel
        mid_fuel = div(min_fuel + max_fuel, 2) + 1
        ore = create_fuel(mid_fuel)
        if ore < total_ore
            min_fuel = mid_fuel
        else
            max_fuel = mid_fuel -1
        end
    end

    println("Second :", mid_fuel)
end

"""
First :13312
Second :82892753
"""

t1()
t2()
