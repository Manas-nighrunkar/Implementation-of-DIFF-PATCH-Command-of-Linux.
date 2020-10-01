import random

# Program to generate test files

letters = ['&', 'd', 'J', 'S', 'e', '9', 'e', '`', 'X', '', 'W', 'x', 'l', 's', '[', 'u', 'E', 'L', 'r', '$', 'M', '}', '^', 'Y', 'w', '|', 'H', 'j', 'E', 'B', '6', '%', 'V', 'v', 'f', '>', ']', 'G', '8', 'b', 'P', '{', '5', '7', 'R', 'f', 'y', 'c', 'Q', 'U', 'A', 'g', 'H', 'K', 'p', 'I', 'F', '!', 'O', '(', '4', '+', 'q', '2', 'h', ':', 'i', 'k', '-', 'F', 'm', 'g', '*', 'a', 'G', '1', 'z', '<', '?', '0', 'o', 'N', 'Z', 'C', 'D', 'n', '3', '.', 'T', ':', 't', '\n']


length = len(letters)
random.shuffle(letters)

number = 0
total = random.randint(2000, 5000)	

with open('source.txt', 'w') as test:
	while(number < total):
		letter = random.randint(0, length-1)
		test.write(letters[letter]);
		number += 1

l = []


with open('source.txt', 'r') as test:
	while True:
		x = test.readline()
		if x == "":
			break
		l.append(x)

			
random.shuffle(l)

with open('destination.txt', 'w') as test:
	i = 0
	while i < len(l):
		test.write(l[i])
		i += 1
			
print(f'Generated "source.txt" and "destination.txt" with {total} number of characters.')
