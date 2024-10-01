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

for filename in onlyfiles:
    print("nothin") 


f.write("#ifndef STRATEGY_CARDS_H\n")
f.write("#define STRATEGY_CARDS_H\n")
f.write("\n#include <iostream>\n\n")


f.write("#endif\n")
f.close()