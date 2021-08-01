subroutine gemini_set_block(blockno)
    implicit none

    integer :: blockno, blockno_com
    common /comblock/ blockno_com

    blockno_com = blockno
    end subroutine gemini_set_block

integer function gemini_get_block()
    implicit none

    integer :: blockno_com
    common /comblock/ blockno_com

    gemini_get_block = blockno_com
    return
end function gemini_get_block

subroutine gemini_unset_block()
    implicit none

    integer :: blockno_com
    common /comblock/ blockno_com

    blockno_com = -1
end subroutine gemini_unset_block
