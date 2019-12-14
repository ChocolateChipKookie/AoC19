file = open("C:\\Projects\\AdventOfCode\\Solutions\\AoC14\\input.txt") do file
    read(file, String)
end
file = split(file, "\r\n")

mutable struct chemical
    name::String
    amount::Int64
end

#Dictionary on how to make different chemicals
#Every chemical key has a tuple value,
#where the first value is the amount of that chemical which will be made during the process
#and the second is the array of chemicals needed for the production
reactions_dict = Dict{String, Tuple{Int64, Array{chemical, 1}}}()

for line in file
    if line == "" continue end
    # Parsing
    substrings = split(line, "=>")
    ingredient_strings = split(strip(substrings[1]), ", ")

    #Create result chemical
    res = split(strip(substrings[2]), " ")
    res = chemical(res[2], parse(Int64, res[1]))

    ingredients = Array{chemical, 1}(undef, 0)

    for substr in ingredient_strings
        #Push chemical to the array
        ingredient = split(strip(substr), " ")
        push!(ingredients, chemical(ingredient[2], parse(Int64, ingredient[1])))
    end

    reactions_dict[res.name] = (res.amount, ingredients)
end

function generate_fuel(quantity::Int64)
    #Create dictionary for all the ingredients that are needed
    needed = Dict{String, Int64}()
    needed["FUEL"] = quantity
    needed["ORE"] = 0
    #Also create dict for available ores (Some residuals will be produced)
    available = Dict{String, Int64}()

    #Do the loop untill there is only one ingredient in the needed dict ("ORE")
    while !(length(needed) == 1)
        #New dict for all the new stuff we need
        new_needed = Dict{String, Int64}()
        new_needed["ORE"] = needed["ORE"]
        #Create all the chemicals we need
        for chem in needed
            #Skip ore as it is the end
            if chem.first == "ORE"
                continue
            end
            #If there is currently not
            if !haskey(available, chem.first)
                available[chem.first] = 0
            end

            #Create at least as many chemicals as we need
            if available[chem.first] < chem.second
                #Specify how many chemicals we need to make
                #And find multiplier as there can be multiple chemicals created
                to_make = chem.second - available[chem.first]
                multiplier = cld(to_make, reactions_dict[chem.first][1])

                #Ingredients for creation destroyed
                for chem_reac in reactions_dict[chem.first][2]
                    if !haskey(new_needed, chem_reac.name)
                        new_needed[chem_reac.name] = 0
                    end
                    new_needed[chem_reac.name] += multiplier* chem_reac.amount
                end

                #Chemicals created
                available[chem.first] += multiplier * reactions_dict[chem.first][1]
            end

            #Get as many chemicals as we need, leave the rest for future production
            available[chem.first] -= chem.second
        end

        #Update the chemicals we need
        needed = new_needed
    end

    #We are left with "ORE" being the only chemical we need
    return needed["ORE"]
end

function t1()
    #How much "ORE" for one "FUEL"
    println("First :", generate_fuel(1))
end

function t2()
    #How much "FUEL" for 1000000000000 "ORE"
    #Binary search
    total_ore = 1000000000000
    min_fuel = 0
    max_fuel = 1000000000000
    mid_fuel = div(min_fuel + max_fuel, 2)

    while min_fuel < max_fuel
        mid_fuel = div(min_fuel + max_fuel, 2) + 1
        ore = generate_fuel(mid_fuel)
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
