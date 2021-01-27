Option Strict On
Option Explicit On

Module mSleep

    Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Integer)

    Public Sub SleepTimer(ByVal dwMilliseconds As Integer)

        Dim Time As Double
        Time = Date.Now.TimeOfDay.TotalSeconds
        While (Date.Now.TimeOfDay.TotalSeconds - Time) < (dwMilliseconds / 1000)
            System.Windows.Forms.Application.DoEvents()
        End While

    End Sub

    Public Sub WaitTime(ByVal nvHalfSecond As Short)

        Dim nvBouble As Double

        nvBouble = 0
        System.Windows.Forms.Application.DoEvents()
        While nvHalfSecond > nvBouble
            Sleep((100))
            nvBouble += 0.2
            System.Windows.Forms.Application.DoEvents()
        End While

    End Sub
End Module