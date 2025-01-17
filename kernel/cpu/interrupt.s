.extern isr_handler
.extern irq_handler

.extern debug_print
isr_common_stub:
    pusha           # Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
    movw %ds, %ax   # Move the value of the DS segment register into AX (16-bit)
    pushl %eax      # Push the contents of AX (extended to 32-bit) onto the stack
    movw $0x10, %ax # Load 0x10 into AX (kernel data segment descriptor)
    movw %ax, %ds   # Set DS to 0x10
    movw %ax, %es   # Set ES to 0x10
    movw %ax, %fs   # Set FS to 0x10
    movw %ax, %gs   # Set GS to 0x10

    cld

    call isr_handler

    popl %eax         # Pop the top of the stack into EAX
    movw %ax, %ds     # Restore DS using the lower 16 bits of EAX
    movw %ax, %es     # Restore ES
    movw %ax, %fs     # Restore FS
    movw %ax, %gs     # Restore GS
    popa              # Restore all general-purpose registers (EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX)
    addl $8, %esp     # Clean up the stack for the pushed error code and ISR number
    sti               # Enable interrupts
    iret              # Return from interrupt

irq_common_stub:
    pusha           # Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
    movw %ds, %ax   # Move the value of the DS segment register into AX (16-bit)
    pushl %eax      # Push the contents of AX (extended to 32-bit) onto the stack
    movw $0x10, %ax # Load 0x10 into AX (kernel data segment descriptor)
    movw %ax, %ds   # Set DS to 0x10
    movw %ax, %es   # Set ES to 0x10
    movw %ax, %fs   # Set FS to 0x10
    movw %ax, %gs   # Set GS to 0x10

    cld

    call irq_handler

    popl %ebx
    movw %bx, %ds     # Restore DS using the lower 16 bits of EBX
    movw %bx, %es     # Restore ES
    movw %bx, %fs     # Restore FS
    movw %bx, %gs     # Restore GS
    popa              # Restore all general-purpose registers (EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX)
    addl $8, %esp     # Clean up the stack for the pushed error code and ISR number
    sti               # Enable interrupts
    iret              # Return from interrupt

.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

# IRQs
.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15

isr0: # Division by Zero
    cli 
    pushl $0
    pushl $0
    jmp isr_common_stub

isr1: # Debug 
    cli 
    pushl $0
    pushl $1
    jmp isr_common_stub

isr2: # Non Maskable Interrupt
    cli 
    pushl $0
    pushl $2
    jmp isr_common_stub

isr3: # Int 3
    cli 
    pushl $0
    pushl $3
    jmp isr_common_stub

isr4: # INTO
    cli 
    pushl $0
    pushl $4
    jmp isr_common_stub

isr5: # Out of Bounds
    cli 
    pushl $0
    pushl $5
    jmp isr_common_stub

isr6: # Invalid Opcode
    cli 
    pushl $0
    pushl $6
    jmp isr_common_stub

isr7: # Coprocessor Not Available
    cli 
    pushl $0
    pushl $7
    jmp isr_common_stub

isr8: # Double Fault
    cli 
    pushl $8
    jmp isr_common_stub

isr9: # Coprocessor Segment Overrun
    cli 
    pushl $0
    pushl $9
    jmp isr_common_stub

isr10: # Bad TSS Exception
    cli 
    pushl $10
    jmp isr_common_stub

isr11: # Segment Not Present
    cli 
    pushl $11
    jmp isr_common_stub

isr12: # Stack Fault
    cli 
    pushl $12
    jmp isr_common_stub

isr13: # General Protection Fault
    cli 
    pushl $13
    jmp isr_common_stub

isr14: # Page Fault
    cli 
    pushl $14
    jmp isr_common_stub

isr15: # Reserved 
    cli 
    pushl $0
    pushl $15
    jmp isr_common_stub

isr16: # Floating Point
    cli 
    pushl $0
    pushl $16
    jmp isr_common_stub

isr17: # Alignment Check
    cli 
    pushl $17
    jmp isr_common_stub

isr18: # Machine Check
    cli 
    pushl $0
    pushl $18
    jmp isr_common_stub

isr19: # Reserved
    cli 
    pushl $0
    pushl $19
    jmp isr_common_stub

isr20: # Reserved
    cli 
    pushl $0
    pushl $20
    jmp isr_common_stub

isr21: # Reserved
    cli 
    pushl $0
    pushl $21
    jmp isr_common_stub

isr22: # Reserved
    cli 
    pushl $0
    pushl $22
    jmp isr_common_stub

isr23: # Reserved
    cli 
    pushl $0
    pushl $23
    jmp isr_common_stub

isr24: # Reserved
    cli 
    pushl $0
    pushl $24
    jmp isr_common_stub

isr25: # Reserved
    cli 
    pushl $0
    pushl $25
    jmp isr_common_stub

isr26: # Reserved
    cli 
    pushl $0
    pushl $26
    jmp isr_common_stub

isr27: # Reserved
    cli 
    pushl $0
    pushl $27
    jmp isr_common_stub

isr28: # Reserved
    cli 
    pushl $0
    pushl $28
    jmp isr_common_stub

isr29: # Reserved
    cli 
    pushl $0
    pushl $29
    jmp isr_common_stub

isr30: # Reserved
    cli 
    pushl $0
    pushl $30
    jmp isr_common_stub

isr31: # Reserved
    cli 
    pushl $0
    pushl $31
    jmp isr_common_stub

# IRQ handlers
irq0:
    cli 
    pushl $0
    pushl $32
    jmp irq_common_stub

irq1:
    cli 
    pushl $1
    pushl $33
    jmp irq_common_stub

irq2:
    cli 
    pushl $2
    pushl $34
    jmp irq_common_stub

irq3:
    cli 
    pushl $3
    pushl $35
    jmp irq_common_stub

irq4:
    cli 
    pushl $4
    pushl $36
    jmp irq_common_stub

irq5:
    cli 
    pushl $5
    pushl $37
    jmp irq_common_stub

irq6:
    cli 
    pushl $6
    pushl $38
    jmp irq_common_stub

irq7:
    cli 
    pushl $7
    pushl $39
    jmp irq_common_stub

irq8:
    cli 
    pushl $8
    pushl $40
    jmp irq_common_stub

irq9:
    cli 
    pushl $9
    pushl $41
    jmp irq_common_stub

irq10:
    cli 
    pushl $10
    pushl $42
    jmp irq_common_stub

irq11:
    cli 
    pushl $11
    pushl $43
    jmp irq_common_stub

irq12:
    cli 
    pushl $12
    pushl $44
    jmp irq_common_stub

irq13:
    cli 
    pushl $13
    pushl $45
    jmp irq_common_stub

irq14:
    cli 
    pushl $14
    pushl $46
    jmp irq_common_stub

irq15:
    cli 
    pushl $15
    pushl $47
    jmp irq_common_stub