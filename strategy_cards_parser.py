from os import listdir
from os.path import isfile, join


# The following script takes all strategy cards and generates a .h file containing
# all cards into structures to be implemented elsewhere


mypath = "strategy cards/"
onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]

print("Parsing /strategy cards/")

strategy_card_list = []



# Begin creating strategy_cards.h
print("Generating strategy_cards.h file")
f = open("strategy_cards.h","w")



f.write("#ifndef STRATEGY_CARDS_H\n")
f.write("#define STRATEGY_CARDS_H\n")


f.write("struct strategy_card\n")
f.write("{\n")
f.write("   char composition[16];\n")
f.write("   char strategy_compact[550];\n")
f.write("   float hold_percentage;\n")
f.write("};\n\n")

f.write("struct strategy_card strategy_card_array[] ={\n")

for filename in onlyfiles:
    # Read from cards
    f2 = open("strategy cards/" + filename, "r")
    buff_strategy = f2.readline()
    house_edge = f2.readline()

    f.write("{\"" + filename + "\",\"" + buff_strategy[:-1] + "\"," + house_edge + "},\n")

    # Write to strategy_cards.h
    #f.write("int " + )



    print(buff_strategy,house_edge)
    f2.close()
f.write("{\"A0H0M0L0S0\",\"123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\",99.99},\n")
f.write("{\"" + "end" + "\",\"" + "end" + "\"," + "999.99" + "}};\n")

f.write("int strategy_card_array_size = " + str(len(onlyfiles) + 1) + ";\n\n")

f.write("#endif\n")
f.close()




#
#struct strategy_card
#{
#    char composition[16];
#    char strategy_compact[550];
#    float hold_percentage;
#};

#struct strategy_card strategy_card_array[] ={
#    {"A0H0M0L0S0", "00100010101010110100101", -9.13}
#};

