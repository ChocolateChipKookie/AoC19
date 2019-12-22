using DataStructures
using OffsetArrays

function t1()
    #Loading maze
    file = split(read("C:\\Projects\\AdventOfCode\\Solutions\\AoC22\\input.txt", String), "\r\n")
    file = filter!(x-> length(x) > 0, file)

    #Creating deck
    deck_size = 10007
    deck = [0:1:(deck_size - 1);]

    #Shuffling
    for line in file
        tokens = split(line)
        #Cutting
        if tokens[1] == "cut"
            position = mod1(parse(Int64, tokens[2]) + 1, length(deck))
            new_deck = Array{Int64, 1}(undef, length(deck))
            for i in 1:length(deck)
                new_deck[i] = deck[position]
                position = mod1(position + 1, length(deck))
            end
            deck = new_deck
        #Deal
        elseif tokens[2] == "into"
            reverse!(deck)
        #Deal with increment
        else
            increment = parse(Int64, tokens[4])
            deck_size = length(deck)
            new_deck = fill(-1, deck_size)
            index = 1
            for i in 1:deck_size
                new_deck[index] = deck[i]
                index = mod1(index + increment, deck_size)
            end
            deck = new_deck
        end
    end

    println("First = $(findall(x->x == 2019, deck)[1] - 1)")
end

#This is a lot of math that I did not come up with. I now understand it but I totally needed help.
function t2()
    #Loading instructions
    file = split(read("C:\\Projects\\AdventOfCode\\Solutions\\AoC22\\input.txt", String), "\r\n")
    file = filter!(x-> length(x) > 0, file)

    #Variables
    deck_size::BigInt = 119315717514047
    repeats::BigInt = 101741582076661
    card_pos::BigInt = 2020

    #Increment and offset
    increment::BigInt = 1
    offset::BigInt = 0

    #Creating the modulo math thingie
    for line in file
        tokens = split(line)
        #Cutting
        if tokens[1] == "cut"
            offset += increment * parse(Int128, tokens[2])
        #Deal
        elseif tokens[2] == "into"
            increment = -increment
            offset += increment
        #Deal with increment
        else
            increment *= invmod(parse(Int128, tokens[4]), deck_size)
        end
    end

    #Lots of modulo math
    total_increment::BigInt = powermod(increment, repeats, deck_size)
    total_offset::BigInt = offset * (1 - total_increment) * invmod(1 - increment, deck_size)

    #Result
    result::BigInt = mod(total_offset + total_increment * card_pos, deck_size)
    println("Second = $result")
end

"""
First = 6638
Second = 77863024474406
"""

t1()
t2()
