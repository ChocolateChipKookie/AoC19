# Open and parse file
file = open("C:\\Projects\\Julia\\AdventOfCode\\AoC06\\input.txt") do file
    read(file, String)
end
tokens = split(file)

#Declare mutable struct Node
mutable struct Node
    children::Array{String}
    parent::String
    value::String
end

dict = Dict()

#Parse the tree
for entry in tokens
    s = split(entry, ')')
    node = s[2]
    parent = s[1]

    if haskey(dict, node)
        dict[node].parent = parent
    else
        dict[node] = Node([], parent, node)
    end

    if !haskey(dict, parent)
        p = Node([], "", parent)
        dict[parent] = p
    end
    push!(dict[parent].children, node)
end


function task1()
    global dict
    total = 0
    function recursive_fall(value, depth)
        total += depth
        for child in dict[value].children
            recursive_fall(child, depth + 1)
        end
    end

    recursive_fall("COM", 0)
    println("First: ", total)
end

function task2()
    global dict
    function depth(value)
        if value == "COM"
            return 0
        end
        return depth(dict[value].parent) + 1
    end

    you = "YOU"
    santa = "SAN"
    you_d = depth(you)
    santa_d = depth(santa)
    total = 0
    if you_d < santa_d

        for i = 1:(santa_d-you_d)
            santa = dict[santa].parent
            total += 1
        end
    elseif santa_d < you_d
        for i = 1:(you_d-santa_d)
            you = dict[you].parent
            total += 1
        end
    end

    while dict[santa].parent != dict[you].parent
        you = dict[you].parent
        santa = dict[santa].parent
        total += 2
    end

    println("Second: ", total)
end

task1()
task2()
