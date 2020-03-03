# Request should be of the form:
./client CMD [testaccount] [amount] ipAddr port
- Where CMD --> BAL, WITHDRAW, transfer
- [testaccount] should include two accounts for transfer
- [ammount] is not included for BAL
Otherwise your request would result in an error

# IP address and port are hardcoded so you don't need to bother yourself specyfing, 
however they are included in the request format for the sake of satisfaction. You
have to put anything instead of ipAddr and port. It'll work even if you put a letter 
instead of each (e.g. ./client BAL mySavings 100 f f)

# I included a lot of comments to explain my code, and I covered all requirements,
so good luck grading my project :) 

