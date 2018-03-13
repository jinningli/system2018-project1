.section .data

s1:.string "Oh, \n"
s2:.string "interesting!\n"

.section .text
.globl _start

_start:

#print oh
movl $4, %eax
movl $1, %ebx
movl $s1, %ecx
movl $5, %edx
int $0x80

#print interesting
movl $4, %eax
movl $1, %ebx
movl $s2, %ecx
movl $13, %edx
int $0x80

movl $1, %eax
int $0x80
