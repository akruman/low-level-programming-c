; creates new entry:    label: dw pointer_to_prev, db key, 0
; label colon_head labels last entry
; example:
; colon "a", aaa
; colon "b", bbb
%macro colon 2 ; key, label
    %ifstr %1
        %define colon_head %2
        %2: ; label
        %ifndef colon_prev
            %xdefine colon_prev %%colon
            colon_prev:
            dq 0 ; first entry
        %else
            %xdefine colon_cur %%colon
            colon_cur:
            dq colon_prev ; points to prev entry
            %xdefine colon_prev colon_cur
        %endif     
        db %1,0 ; key
    %else
        %fatal "macro colon first arg is key and should be a string" 
    %endif ; ifstr
    
%endmacro

