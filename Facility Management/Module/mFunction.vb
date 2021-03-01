Imports VB = Microsoft.VisualBasic

Module mFunction

    Public Declare Function GetDiskFreeSpaceEx Lib "kernel32" Alias "GetDiskFreeSpaceExA" (ByVal lpRootPathName As String, ByRef lpFreeBytesAvailableToCaller As LARGE_INTEGER, ByRef lpTotalNumberOfBytes As LARGE_INTEGER, ByRef lpTotalNumberOfFreeBytes As LARGE_INTEGER) As Long

    Private Declare Auto Function GetShortPathName Lib "kernel32" (ByVal longPath As String, ByVal shortPath As System.Text.StringBuilder, ByVal shortBufferSize As Integer) As Integer

    Public Const DIRECTORY As Short = 1 'Indicates that the path minus the disk drive letter and filename should be returned in svReturnString. When this option is used with a UNC path, ParsePath returns the path without the server and shared device name, and without the filename if one was specified. For example, the UNC path \\TheServer\TheSharedDevice\TheApp\TheFile.exe is returned in svReturnString as \TheApp\. 
    Public Const DISK As Short = 2 'Indicates that the disk drive designation (drive letter followed by a colon) should be returned in svReturnString. When this option is used with a UNC path, ParsePath returns the server and shared device name. For example, the UNC path \\TheServer\TheSharedDevice\TheApp\TheFile.exe is returned in svReturnString as \\TheServer\TheSharedDevice. 
    Public Const EXTENSION_ONLY As Short = 3 'Indicates that the file extension should be returned in svReturnString. It does not include the period.
    Public Const FILENAME As Short = 4 'Indicates that the complete filename (that is, with its file extension) should be returned in svReturnString.
    Public Const FILENAME_ONLY As Short = 5 'Indicates that the filename only (that is, without its file extension) should be returned in svReturnString.
    Public Const PATH As Short = 6 'Indicates that the path minus the filename should be returned in svReturnString. This option differs from DIRECTORY in that the drive designation (if specified in szPath) is included in the returned path. When szPath specifies a UNC path the server and shared device name are included in the returned path. For example, the UNC path \\TheServer\TheSharedDevice\TheApp\TheFile.exe is returned in svReturnString as \\TheServer\TheSharedDevice\TheApp\.
    Public Const PATH_FILENAME_ONLY As Short = 7

    Structure LARGE_INTEGER
        Dim lowpart As Long
        Dim highpart As Long
    End Structure

    Public Function DateFile(ByVal svFile As String) As Double

        Dim svDate As String = CStr(FileDateTime(svFile))

        DateFile = Val(Mid(svDate, 7, 4) + Mid(svDate, 4, 2) + Mid(svDate, 1, 2) + Mid(svDate, 12, 2) + Mid(svDate, 15, 2) + Mid(svDate, 18, 2))

    End Function

    Public Function CLargeInt(ByVal Lo As Long, ByVal Hi As Long) As Double

        'This function converts the LARGE_INTEGER data type to a double

        Dim dblLo As Double, dblHi As Double

        If Lo < 0 Then
            dblLo = 2 ^ 32 + Lo
        Else
            dblLo = Lo
        End If

        If Hi < 0 Then
            dblHi = 2 ^ 32 + Hi
        Else
            dblHi = Hi
        End If

        CLargeInt = dblLo + dblHi * 2 ^ 32

    End Function

    Public Function GetFreeSpace(ByVal szPath As String) As Double

        Dim lResult As Long
        Dim liAvailable As LARGE_INTEGER
        Dim liTotal As LARGE_INTEGER
        Dim liFree As LARGE_INTEGER
        'Dim dblAvailable As Double
        'Dim dblTotal As Double
        'Dim dblFree As Double

        Try

            lResult = GetDiskFreeSpaceEx(szPath, liAvailable, liTotal, liFree)

            'Convert the return values from LARGE_INTEGER to doubles
            Return CLargeInt(liAvailable.lowpart, liAvailable.highpart)
            'dblAvailable = CLargeInt(liAvailable.lowpart, liAvailable.highpart)
            'dblTotal = CLargeInt(liTotal.lowpart, liTotal.highpart)
            'dblFree = CLargeInt(liFree.lowpart, liFree.highpart)

        Catch ex As Exception

            Return -1

        End Try

    End Function


    'Public Sub IO_File_WriteAllText(ByVal path As String, ByVal contents As String)

    '    If IO.File.Exists(path) Then IO.File.Delete(path)

    '    Dim bytes(contents.Length - 1) As Byte

    '    For index = 0 To Len(contents) - 1
    '        Dim AscII As Integer = Asc(Mid(contents, index + 1, 1))
    '        bytes(index) = CByte(AscII)
    '    Next
    '    '

    '    IO.File.WriteAllBytes(path, bytes)

    'End Sub


    'Public Function IO_File_ReadAllText(ByVal path As String) As String

    '    'If IO.File.Exists(path) Then IO.File.Delete(path)

    '    Dim bytes() As Byte = IO.File.ReadAllBytes(path)

    '    Dim contents As String = ""

    '    For Each x As Byte In bytes

    '        contents = contents + CChar(ChrW(x))
    '    Next

    '    Return contents

    'End Function

    Public Function IsDirectoryWritable(ByVal path As String) As Boolean
        Dim info As New IO.DirectoryInfo(path)
        Return (info.Attributes And IO.FileAttributes.ReadOnly) <> IO.FileAttributes.ReadOnly
    End Function

    Public Function RemoveFirstFolder(ByVal szString As String) As String

        If Left(szString, 1) = "\" Then
            szString = Mid(szString, 2, szString.Length - 1)
        End If

        Dim nPosSlash, nLenPath As Integer

        nPosSlash = InStr(szString, "\")
        nLenPath = Len(szString)

        If nPosSlash > 0 Then
            Return Mid(szString, nPosSlash + 1, nLenPath - 1)
        Else
            Return szString
        End If

    End Function

    Public Function ParsePath(ByVal szPath As String, ByVal nOperation As Short) As String

        Dim nPosSlash, nPosPoint, nLenPath As Integer

        nPosPoint = InStrRev(szPath, ".") - 1
        nPosSlash = InStrRev(szPath, "\")
        nLenPath = Len(szPath)

        If nOperation = EXTENSION_ONLY Then
            Return VB.Right(szPath, nLenPath - nPosPoint)
        ElseIf nOperation = FILENAME_ONLY Then
            If nPosPoint = -1 Or nPosSlash > nPosPoint Then 'pas d'extension
                Return VB.Right(szPath, nLenPath - nPosSlash)
            Else
                Return Mid(szPath, nPosSlash + 1, nPosPoint - nPosSlash)
            End If
        ElseIf nOperation = FILENAME Then
            Return VB.Right(szPath, nLenPath - nPosSlash)
        ElseIf nOperation = PATH Then
            Return VB.Left(szPath, nPosSlash)
        ElseIf nOperation = PATH_FILENAME_ONLY Then
            Return VB.Left(szPath, nPosPoint)
        Else
            Return szPath
        End If


    End Function

    Public Function StrCompTextVersions(ByVal version1 As String, ByVal version2 As String) As Integer

        StrCompTextVersions = 0

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

    Public Function DirSize(ByVal d As IO.DirectoryInfo) As Long
        Dim Size As Long = 0
        ' Add file sizes.
        Dim fis As IO.FileInfo() = d.GetFiles()
        Dim fi As IO.FileInfo
        For Each fi In fis
            Size += fi.Length
        Next fi
        ' Add subdirectory sizes.
        Dim dis As IO.DirectoryInfo() = d.GetDirectories()
        Dim di As IO.DirectoryInfo
        For Each di In dis
            Size += DirSize(di)
        Next di
        Return Size
    End Function 'DirSize

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

    Public Function ShortPathName(ByVal szPathName As String) As String

        Dim shortName As New System.Text.StringBuilder(260)

        GetShortPathName(szPathName, shortName, shortName.Capacity)

        Return shortName.ToString

    End Function


End Module
