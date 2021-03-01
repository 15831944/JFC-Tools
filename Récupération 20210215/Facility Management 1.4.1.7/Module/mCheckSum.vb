
Option Strict Off
Option Explicit On

Module mCheckSum

    ' Ajout de DoEvents

    '|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    '|     CRC32 Check/Append      \
    '| Tutorial by E.B.ArtSoft@     \
    '| email : EBArtSoft@aol.com     \
    '| Source: Original CRC32 module  \
    '|         coded by Neo            \
    '|         http://vbcode.8m.com/    \
    '|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

    Private ReadOnly Crc32Table(255) As Integer

    Private Function InitCrc32(Optional ByVal Seed As Integer = &HEDB88320, Optional ByVal Precondition As Integer = &HFFFFFFFF) As Integer
        'Declaration des variables
        Dim lTempCrc32 As Integer
        Dim iBytes As Short
        Dim iBits As Short
        Dim lCrc32 As Integer
        'Boucle commence a 0 fini a 255 soit 256 cycles
        For iBytes = 0 To 255
            'CRC temporaire = compteur de cycle
            lCrc32 = iBytes
            'Effectue un decalage de bit vers la droite
            For iBits = 0 To 7
                'Masque le bits de poid faible
                lTempCrc32 = lCrc32 And &HFFFFFFFE
                'Decale les bit vers la droite
                lTempCrc32 \= &H2
                'Masque le bit de poid fort
                lTempCrc32 = lTempCrc32 And &H7FFFFFFF
                'Verifie si le CRC = 0 et mix avec la valeur Seed
                If (lCrc32 And &H1) <> 0 Then
                    lCrc32 = lTempCrc32 Xor Seed
                Else
                    lCrc32 = lTempCrc32
                End If
            Next
            'Ajoute le CRC dans la table
            Crc32Table(iBytes) = lCrc32
        Next
        'Renvoi la precondition
        InitCrc32 = Precondition
    End Function

    Private Function AddCrc32(ByRef Data() As Byte, ByVal CRC32 As Integer) As Integer
        'Declaration des variables
        Dim lTableValue As Integer
        Dim iCounter As Integer
        Dim bCharValue As Byte
        Dim lAccValue As Integer
        Dim lIndex As Integer

        Dim IndexMax As Integer = UBound(Data)

        'On vas verifier toute les données du fichier
        'Donc boucle de 0 à [Fin du fichier]
        For iCounter = 0 To IndexMax

            'Var temporaire = donnée du fichier
            'à la position icounter
            bCharValue = Data(iCounter)
            'Masque les bits 0 à 255 (poid faible)
            lAccValue = CRC32 And &HFFFFFF00
            'Divise par 256 (decalage vers la droite)
            lAccValue \= &H100
            'Masque les bits de poid fort
            lAccValue = lAccValue And &HFFFFFF
            'Recupere le module de 256
            lIndex = CRC32 And &HFF
            'Combine avec notre var temp
            lIndex = lIndex Xor bCharValue
            'Recupere le CRC dans notre table
            'Selon l'index donné par notre calcule
            lTableValue = Crc32Table(lIndex)
            'Puis on combine avec notre valeur
            CRC32 = lAccValue Xor lTableValue

        Next
        'Renvoi le CRC
        AddCrc32 = CRC32

    End Function

    Private Function GetCrc32(ByVal CRC32 As Integer) As Integer
        'Renvoi le ou exclusif du CRC
        GetCrc32 = CRC32 Xor &HFFFFFFFF
    End Function

    Public Function Append(ByVal szFileName As String, Optional ByVal CreateCRC As Boolean = True) As String

        If StrComp(ParsePath(Application.ExecutablePath, FILENAME), ParsePath(szFileName, FILENAME), CompareMethod.Text) = 0 Then
            CreateCRC = False
        End If

        Dim svFileOldCRC As String = ParsePath(szFileName, PATH_FILENAME_ONLY) + FormBox.mExtFileCRC
        If IO.File.Exists(svFileOldCRC) Then
            IO.File.Delete(svFileOldCRC)
        End If
        Dim svFileCRC As String = szFileName + FormBox.mExtFileCRC

        If IO.File.Exists(svFileCRC) And CreateCRC Then

            If DateTime.Compare(IO.File.GetLastWriteTime(svFileCRC), IO.File.GetLastWriteTime(szFileName)) = 0 Then
                Return (IO.File.ReadAllText(svFileCRC, System.Text.Encoding.Default))

            Else
                If MsgBox("File date error: " + svFileCRC + vbNewLine + vbNewLine + "Do you want to create it again?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton1, MsgBoxStyle), "CRC error") = MsgBoxResult.No Then
                    Return (IO.File.ReadAllText(svFileCRC, System.Text.Encoding.Default))
                Else
                    IO.File.Delete(svFileCRC)
                End If
            End If

        ElseIf IO.File.Exists(svFileCRC) Then
            IO.File.Delete(svFileCRC)
        End If


        'Declaration des variables
        Dim lCrc32Value As Integer
        Dim nCount As Integer = 0
        Dim data() As Byte
        Dim buffer As Integer = 1024 * 1024

        'Si erreur on vas au label "Fin"
        'On Error GoTo Fin

        Append = ""

        'Ajout ProgressBar CRC
        FormBox.ToolStripStatusLabelFM.Text = "Check CRC: " + ParsePath(szFileName, FILENAME_ONLY)

        'Ouvre le fichier
        Dim FileNumber As Short = CShort(FreeFile())
        FileOpen(FileNumber, szFileName, OpenMode.Binary, OpenAccess.Read)
        'Recupere la taille du fichier
        Dim FileSize As Integer = CInt(LOF(FileNumber))


        System.Windows.Forms.Application.DoEvents()

        'Seulement si le fichier n'est pas vide
        If FileSize <= 0 Then
            'Fin de la procedure
            'Le fchier est vide
            FileClose(FileNumber)
            Exit Function
        End If

        'Initialise le tableau du CRC
        lCrc32Value = InitCrc32()

        'Calcule le CRC
        While nCount < FileSize

            Dim nValue As Integer = CInt((nCount / FileSize) * 100)

            If nValue <> FormBox.ToolStripProgressBar1.Value Then
                FormBox.ToolStripProgressBar1.Value = nValue
                System.Windows.Forms.Application.DoEvents()
            End If

            If FileSize - nCount < buffer Then
                buffer = FileSize - nCount
            End If

            'Charge les données du fichier dans le buffer
            ReDim data(buffer - 1)
            FileGet(FileNumber, data)

            nCount += buffer

            lCrc32Value = AddCrc32(data, lCrc32Value)

        End While

        FileClose(FileNumber)

        FormBox.ToolStripProgressBar1.Value = 100

        'Ajouste le CRC par XOR
        lCrc32Value = GetCrc32(lCrc32Value)

        'Info
        Append = CStr(Hex(lCrc32Value))

        If CreateCRC Then

            IO.File.WriteAllText(svFileCRC, Append, System.Text.Encoding.Default)

            IO.File.SetCreationTime(svFileCRC, IO.File.GetCreationTime(szFileName))
            IO.File.SetLastAccessTime(svFileCRC, IO.File.GetLastAccessTime(szFileName))
            IO.File.SetLastWriteTime(svFileCRC, IO.File.GetLastWriteTime(szFileName))

        End If

        'Ajout ProgressBar CRC
        FormBox.ToolStripStatusLabelFM.Text = Nothing

    End Function

    Public Function AppendString(ByVal svData As String) As String
        'Declaration des variables
        Dim lCrc32Value As Integer
        Dim FileData() As Byte
        Dim FileSize As Integer
        Dim nvData As Integer = 1

        FileSize = Len(svData)

        ReDim FileData(FileSize - 1)

        While nvData < FileSize
            System.Windows.Forms.Application.DoEvents()
            FileData(nvData - 1) = CByte(Asc(Mid(svData, nvData, 2)))
            nvData += 2
        End While

        'Initialise le tableau du CRC
        lCrc32Value = InitCrc32()
        'Calcule le CRC
        lCrc32Value = AddCrc32(FileData, lCrc32Value)
        'Ajouste le CRC par XOR
        lCrc32Value = GetCrc32(lCrc32Value)

        'Info
        AppendString = CStr(Hex(lCrc32Value))

    End Function
End Module