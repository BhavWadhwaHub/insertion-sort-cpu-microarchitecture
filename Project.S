.section .note.GNU-stack,""
.global insertion_sort_asm
.global insertion_sort_simd_asm
.text

insertion_sort_asm:
    mov $1,%r8   #index value=1
    cmp $1,%rsi  #handling edge cases
    jle .exit
.main_loop:
    cmp %rsi,%r8      #i<size
    jge .exit

    mov (%rdi,%r8,8),%rcx     #arr[i]=rcx
    mov %r8,%r9         
    sub $1,%r9        #j=i-1
.inner_loop:
    cmp $0,%r9     #j<0
    jl .case1

    mov (%rdi,%r9,8),%rdx    #rdx=arr[j]
    cmp %rcx,%rdx      # comparing arr[j] with value
    jle .case1

    mov %rdx,8(%rdi,%r9,8)     
    sub $1,%r9     #j=j-1
    jmp .inner_loop
.case1:
    mov %rcx,8(%rdi,%r9,8)     
    add $1,%r8  #i+=1
    jmp .main_loop 
.exit:
    ret




insertion_sort_simd_asm:
    cmp $1,%rsi
    jle .done
    mov $1,%r8
.loop:
    cmp %rsi,%r8
    jge .done

    mov (%rdi,%r8,8),%rcx
    mov %r8,%r9
.find_pos:
    dec %r9
    cmp $0,%r9
    jl .shift_setup
    cmp %rcx,(%rdi,%r9,8)
    jg .find_pos

.shift_setup:
    inc %r9
    mov %r8,%r10
.shift:
    dec %r10
    cmp %r9,%r10
    jl .insert_key

    mov %r10,%rax
    sub %r9,%rax
    cmp $3,%rax
    jl .scalar_one

    vmovdqu -24(%rdi,%r10,8),%ymm0
    vmovdqu %ymm0,-16(%rdi,%r10,8)

    sub $3,%r10
    jmp .shift
.scalar_one:
    mov (%rdi,%r10,8),%rax
    mov %rax,8(%rdi,%r10,8)
    jmp .shift
.insert_key:
    mov %rcx,(%rdi,%r9,8)
    inc %r8
    jmp .loop
.done:
    vzeroupper
    ret