Option Strict Off
Option Explicit On

Module mCheckSum

    '|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    '|     CRC32 Check/Append      \
    '| Tutorial by E.B.ArtSoft@     \
    '| email : EBArtSoft@aol.com     \
    '| Source: Original CRC32 module  \
    '|         coded by Neo            \
    '|         http://vbcode.8m.com/    \
    '|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

    Private Crc32Table(255) As Integer

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
                lTempCrc32 = lTempCrc32 \ &H2
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
        'On vas verifier toute les données du fichier
        'Donc boucle de 0 à [Fin du fichier]


        For iCounter = 0 To UBound(Data)

            'Var temporaire = donnée du fichier
            'à la position icounter
            bCharValue = Data(iCounter)
            'Masque les bits 0 à 255 (poid faible)
            lAccValue = CRC32 And &HFFFFFF00
            'Divise par 256 (decalage vers la droite)
            lAccValue = lAccValue \ &H100
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

    Public Function Append(ByVal FileName As String) As String
        'Declaration des variables
        Dim lCrc32Value As Integer
        Dim nCount As Integer = 0
        Dim data() As Byte
        Dim buffer As Integer = 1024 * 1024

        'Si erreur on vas au label "Fin"
        'On Error GoTo Fin

        Append = ""

        'Ouvre le fichier
        Dim FileNumber As Short = CShort(FreeFile())
        FileOpen(FileNumber, FileName, OpenMode.Binary, OpenAccess.Read)
        'Recupere la taille du fichier
        Dim FileSize As Integer = CInt(LOF(FileNumber))
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

            If FileSize - nCount < buffer Then
                buffer = FileSize - nCount
            End If

            'Charge les données du fichier dans le buffer
            ReDim data(buffer - 1)
            FileGet(FileNumber, data)

            nCount = nCount + buffer

            lCrc32Value = AddCrc32(data, lCrc32Value)

        End While

        FileClose(FileNumber)

        'Ajouste le CRC par XOR
        lCrc32Value = GetCrc32(lCrc32Value)

        'Info
        Append = CStr(Hex(lCrc32Value))

        'Exit Function
        'Fin: MsgBox Err.Description, vbCritical
        '     Err = 0
    End Function

    Public Function AppendString(ByVal svData As String) As String
        'Declaration des variables
        Dim lCrc32Value As Integer
        Dim FileData() As Byte
        Dim FileSize As Integer
        Dim nvData As Integer = 1

        AppendString = ""
        FileSize = Len(svData)

        ReDim FileData(FileSize - 1)

        While nvData < FileSize
            FileData(nvData - 1) = CByte(Asc(Mid(svData, nvData, 2)))
            nvData = nvData + 2
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