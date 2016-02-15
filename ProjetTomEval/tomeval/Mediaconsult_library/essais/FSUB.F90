! Fortran subroutine which is called from a C main program.
! The source is structured so that it can be built either
! as a DLL or just mixed in with the C source directly.
!
SUBROUTINE FSUB (INT_ARG, STR_IN, STR_OUT)
IMPLICIT NONE

! If we're building a DLL, specify that DLL_ROUT is 
! exported to a DLL.  Key this off of the preprocessor
! symbol _DLL which is automatically defined if we're
! building a DLL.

!DEC$ IF DEFINED (_DLL)
!DEC$ ATTRIBUTES DLLEXPORT :: FSUB
!DEC$ END IF

INTEGER, INTENT(IN) :: INT_ARG
CHARACTER(*), INTENT(IN) :: STR_IN
CHARACTER(*), INTENT(OUT) :: STR_OUT

! This routine converts INT_ARG to a decimal string.
! appends the string value to STR_IN and stores it
! in STR_OUT. A trailing NUL is added to keep C
! happy.
!
! Note that there are implicit length arguments following
! the addresses of each CHARACTER argument - see the CMAIN
! source for more details.

CHARACTER*5 INT_STR

WRITE (INT_STR,'(I5.5)')INT_ARG

STR_OUT = STR_IN // INT_STR // CHAR(0)

RETURN
END 
