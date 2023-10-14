## Test Program to show the simulator works with different assembly files

	.text
main:
	
## read the string S:
    la $a0, answer
	li $v0, 4		# Print String
	syscall

exit:				# exit the program
	li $v0, 10		# load "exit" code into $v0.
	syscall			# make the system call.

	.data
answer: ".asciiz" This program still works!
