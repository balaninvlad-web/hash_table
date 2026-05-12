;:================================================
;: My_avx_strcmp.asm                (c)VOV4IK,2026
;:================================================

; My_printf – аналог strcmp для Windows x64 (NASM)
; необходима выровненость строк по 32 байта
; Используются AVX2 инструкции
; Прототип: int MyAvxStrcmp (const char* a, const char* b);
; Возвращает 0, если равны; иначе разницу первых несовпадающих символов.
;nasm -f win64 My_avx_strcmp.asm -o My_avx_strcmp.obj -l "My_avx_strcmp.lst"

section .text
global MyAvxStrcmp

MyAvxStrcmp:
            ; ymm0 - a, ymm1 - b

            vmovdqa ymm0, [rdi]
    	    vmovdqa ymm1, [rsi]

            ; Сравнивание побайтово (байты равны -> получается 0xFF в ymm2)
            vpcmpeqb ymm2, ymm0, ymm1

            vpmovmskb eax, ymm2

            ; Если все 32 бита установлены (0xFFFFFFFF) -> строки равны
            cmp eax, 0xFFFFFFFF
            je .equal

            not eax

            tzcnt ecx, eax

            movzx edx, byte [rsi + rcx]
            movzx eax, byte [rdi + rcx]

            sub  edx, eax                 
            mov  eax, edx
            ret

.equal:
            xor eax, eax
            ret
            
