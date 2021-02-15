Option Strict On
Option Explicit On

Module mFunction

    Public Declare Function GetTickCount Lib "kernel32" () As Integer
    Public Declare Function GetAsyncKeyState Lib "user32.dll" (ByVal vKey As Integer) As Integer
 
	Public Function ConvRGB(ByVal Value As String) As Integer
		Dim ColorRGB() As String
		
		ColorRGB = Split(Value, ",")
        ConvRGB = RGB(CInt(Val(ColorRGB(0))), CInt(Val(ColorRGB(1))), CInt(Val(ColorRGB(2))))
		
	End Function
	
    Public Function ConvertOctets(ByVal Octets As Double) As String

        If Octets >= (1024 ^ 4) Then
            ConvertOctets = Virgul(Octets / (1024 ^ 4)) & "To"
        ElseIf Octets >= (1024 ^ 3) Then
            ConvertOctets = Virgul(Octets / (1024 ^ 3)) & "Go"
        ElseIf Octets >= (1024 ^ 2) Then
            ConvertOctets = Virgul(Octets / (1024 ^ 2)) & "Mo"
        ElseIf Octets >= 1024 Then
            ConvertOctets = Virgul(Octets / 1024) & "Ko"
        Else
            ConvertOctets = Trim(CStr(Octets)) & "o"
        End If

    End Function
	
    Private Function Virgul(ByVal Val_Renamed As Double) As String

        Dim PosVirgul As Integer
        Dim LenVal As Integer

        LenVal = Len(Trim(CStr(Int(Val_Renamed))))
        If LenVal >= 3 Then
            PosVirgul = 0
        Else
            PosVirgul = 3 - LenVal
        End If
        Virgul = Trim(CStr(System.Math.Round(Val_Renamed, PosVirgul)))

    End Function

	Public Function ConvertCount(ByVal Num As Integer, ByVal LenCount As Short) As String

        Dim nLen As Integer, CountChar As Integer
        Dim Char_Renamed As String = ""
		
		ConvertCount = Trim(Str(Num))
		nLen = Len(ConvertCount)
		
		For CountChar = 1 To LenCount - nLen
            Char_Renamed &= "0"
        Next 
		
		ConvertCount = Char_Renamed & ConvertCount
		
	End Function
	
	Public Function Timer2Time(ByVal Temp As Integer) As String
		Dim TempM, TempH, TempS As String
		
		TempH = Trim(CStr(Int(Temp / 3600)))
		TempM = Trim(CStr(Int(Temp / 60) - CDbl(TempH) * 60))
		TempS = Trim(CStr(Temp - CDbl(TempH) * 3600 - CDbl(TempM) * 60))
		
		If Len(TempH) = 1 Then
			TempH = "0" & TempH
		End If
		If Len(TempM) = 1 Then
			TempM = "0" & TempM
		End If
		If Len(TempS) = 1 Then
			TempS = "0" & TempS
		End If
		
		Timer2Time = TempH & ":" & TempM & ":" & TempS
		
	End Function

	Public Function GetFileToText(ByVal PathFileTxt As String) As String
        Dim NumFic As Integer
		Dim svLine As String
        Dim TextFile As String = ""
		
        If IO.File.Exists(PathFileTxt) Then
            NumFic = FreeFile()
            FileOpen(NumFic, PathFileTxt, OpenMode.Input)
            While Not EOF(1)
                svLine = LineInput(NumFic)
                TextFile = TextFile & svLine & vbNewLine
            End While
            FileClose(NumFic)
            GetFileToText = TextFile
        Else
            GetFileToText = ""
        End If
		
	End Function
	
	Public Function FindNumVal(ByVal svString As String) As String
		Dim svTemp As String
		
        If Len(svString) = 0 Then Return "" : Exit Function
		
		Do 
			svTemp = Mid(svString, 1, 1)
			If Asc(svTemp) < 58 And Asc(svTemp) > 47 Then Exit Do
            If Len(svString) = 1 Then Return "" : Exit Function
			svString = Mid(svString, 2, Len(svString))
		Loop 
		
		Do 
			svTemp = Mid(svString, Len(svString), 1)
			If Asc(svTemp) < 58 And Asc(svTemp) > 47 Then Exit Do
			svString = Mid(svString, 1, Len(svString) - 1)
		Loop 
		
		FindNumVal = svString
		
	End Function
	
    Public Function StrReplace(ByVal sValue As String, ByVal StringFrom As String, ByVal StringTo As String) As String

        Dim nPos As Integer

        nPos = InStr(sValue, StringFrom, CompareMethod.Text)
        While nPos > 0
            sValue = Left(sValue, nPos - 1) & StringTo & Right(sValue, Len(sValue) - (nPos + Len(StringFrom) - 1))
            nPos = InStr(sValue, StringFrom)
        End While

        StrReplace = sValue

    End Function

    Public Sub StrRemoveToFile(ByRef StrFind As String, ByRef File As String)

        Dim cut() As String = IO.File.ReadAllLines(File, System.Text.Encoding.Default)
        Dim resultat As String = ""

        For i = 0 To cut.Length - 1
            If InStr(cut(i).ToUpper, StrFind.ToUpper, CompareMethod.Text) <> 1 Then
                resultat &= cut(i) & Environment.NewLine
            End If
        Next i

        IO.File.WriteAllText(File, resultat, System.Text.Encoding.Default)

    End Sub

    Public Function StrCompTextVersions(ByVal version1 As String, ByVal version2 As String) As Integer

        StrCompTextVersions = 0

        If version1 = Nothing Or version2 = Nothing Then
            Return 1
        End If

        ' 1  version1 > version2
        '-1  version1 < version2
        ' 0  version1 = version2

        Try

            Dim m_a() As String = version1.Split(CChar("."))
            Dim m_b() As String = version2.Split(CChar("."))

            For Index = 0 To m_a.Length - 1
                If Val(m_a(Index)) > Val(m_b(Index)) Then
                    StrCompTextVersions = 1
                    Exit For
                ElseIf Val(m_a(Index)) < Val(m_b(Index)) Then
                    StrCompTextVersions = -1
                    Exit For
                End If
            Next

        Catch ex As Exception
            Return 1
        End Try

    End Function

End Module