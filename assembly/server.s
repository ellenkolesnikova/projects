.intel_syntax noprefix
.globl _start

.section .data

    .equ SOCK_STREAM, 1
    .equ AF_INET, 2
    ok_response:
        .string "HTTP/1.0 200 OK\r\n\r\n"
    
    bufGET:
        .space 0, 256
    sockaddr:
        .short 2 # AF_INET
        .short 0x5000 # port 80
        .long 0x00000000 # local host ip addr
        .zero 8 # zeroed out
    buf:
        .space 0, 256
    
    
    

.section .text

_start:
    xor rcx, rcx

_socket:
    xor rdi, rdi
    xor rsi, rsi
    xor rdx, rdx
    mov rdi, AF_INET
    mov rsi, SOCK_STREAM
    mov rdx, 0
    mov rax, 41
    syscall
    mov rbp, rax # store in rbp the socket fd

_bind:
    xor rdi, rdi
    xor rsi, rsi
    xor rdx, rdx
    mov rdi, rbp
    lea rsi, sockaddr
    mov rdx, 16
    xor rax, rax
    mov rax, 49
    syscall

_listen:
    xor rdi, rdi
    xor rsi, rsi
    mov rdi, rbp
    mov rsi, 0
    mov rax, 50
    syscall

_accept:
    xor rdi, rdi
    xor rsi, rsi
    xor rdx, rdx
    mov rdi, rbp
    xor rax, rax
    mov rax, 43
    syscall
    mov rbx, rax

_fork:
    xor rax, rax
    mov rax, 57
    syscall 
    cmp rax, 0
    jz CHILD

    PARENT:
        xor rdi, rdi
        mov rdi, rbx
        xor rax, rax
        mov rax, 3
        syscall
        jmp _accept
    CHILD:
        xor rdi, rdi
        mov rdi, rbp
        xor rax, rax
        mov rax, 3
        syscall




_readrequest:
    xor rdi, rdi
    xor rsi, rsi
    xor rdx, rdx
    mov rdi, rbx # rbx has new accepted port number
    push rdi # for reference later
    lea rsi, buf # read buffer
    mov rdx, 256 # num of chars read
    xor rax, rax
    mov rax, 0
    syscall

_decipherGETrequest:
    xor rcx, rcx
    xor rbx, rbx # index - gets updated to filename end
    xor rdx, rdx # current char
    lea rcx, buf
    mov rbx, 4 # skip the GET 
    LOOP:
        movzx rdx, byte ptr [rcx+rbx]
        cmp rdx, 0x20 # compare to space
        je END
        inc rbx
        jmp LOOP
    END:
        nop

_openGETfile:
    xor rdi, rdi # 0 for read-only
    xor rsi, rsi
    xor rdx, rdx
    xor rax, rax
    lea rdi, [buf+4]
    mov byte ptr [buf+rbx], 0 # filename goes until rbx, calculated in _decipherGETrequest
    mov rax, 2
    syscall

_readGETfile:
    xor rdi, rdi
    xor rsi, rsi
    xor rdx, rdx
    mov rdi, rax # rax has new accepted port number
    lea rsi, bufGET # read buffer
    mov rdx, 256 # num of chars read
    xor rax, rax
    mov rax, 0
    syscall
    mov rbx, rax # save the num of chars in rbx

_closeGETfile:
    # keep rdi the same from _readGETfile
    xor rax, rax
    mov rax, 3
    syscall


_respondOK:
    xor rdi, rdi
    pop rdi # for reference
    xor rsi, rsi
    xor rdx, rdx
    lea rsi, ok_response
    mov rdx, 19
    mov rax, 1
    syscall
    push rdi # for reference later

_respondfilecontents:
    # keep same rdi
    xor rsi, rsi
    xor rdx, rdx
    lea rsi, [bufGET]
    mov rdx, rbx
    mov rax, 1
    syscall


_close:
    xor rdi, rdi
    pop rdi
    xor rax, rax
    mov rax, 3
    syscall



_exit:
    mov rdi, 0
    mov rax, 60
    syscall
