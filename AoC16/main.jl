function calculate_phase(input, result)
    for x in 1:length(input)
        len = x * 4
        result[x] = 0
        for j in 0:(x-1)
            start = x + j
            for i in start:len:length(input)
                result[x] += input[i]
            end

            for i in (start + div(len, 2)):len:length(input)
                result[x] -= input[i]
            end
        end
        result[x] = string(result[x])[end] - '0'
    end
end

function calculate_phase_second_half(input, result)
    #Good old fashioned dynamic programming
    result[length(input)] = input[length(input)]
    for x in (length(input) - 1):-1:div(length(input), 2)
        #As the second half is only a triangular matrix, it is only additions
        #When only adding we can get the last digit with the mod operation
        #Slow way 43.744163 seconds (650.00 M allocations: 29.154 GiB, 10.47% gc time):
        #result[x] = input[x] + result[x+1]
        #result[x] = string(result[x])[end] - '0'
        #Fast way  2.844051 seconds (79 allocations: 99.194 MiB, 0.92% gc time):
        result[x] = mod(input[x] + result[x+1], 10)
    end
end

function t1()
    #Reading file
    file = open("C:\\Projects\\AdventOfCode\\Solutions\\AoC16\\input.txt") do file
        read(file, String)
    end
    input = map(x-> x - '0', collect(strip(file)))
    result = Array{Int64, 1}(undef, length(input))
    #Number of iterations is divisible by 2, this way, the arrays are created only once
    for i in 1:div(100, 2)
        calculate_phase(input, result);
        calculate_phase(result, input)
    end
    #Printing
    println("First: $(prod(map(x->string(x), input[1:8])))")
end



function t2()
    @time begin
    #Reading file
    file = open("C:\\Projects\\AdventOfCode\\Solutions\\AoC16\\input.txt") do file
        read(file, String)
    end
    #Parsing input
    input = map(x-> x - '0', collect(strip(file)))
    #Doing the ugly stuff
    input = repeat(input, 10000)
    result = Array{Int64, 1}(undef, length(input))
    #Parsing offset
    offset = parse(Int64, prod(map(x->string(x), input[1:7])))

    #We only need to calculate the second half of the matrix because the result is there and it is a triangular matrix
    for i in 1:div(100, 2)
        calculate_phase_second_half(input, result)
        calculate_phase_second_half(result, input)
    end

    println("Second: $(prod(map(x->string(x), input[offset + 1:offset + 8])))")
end
end


"""
First: 73127523
Second: 80284420
"""

t1()
t2()
