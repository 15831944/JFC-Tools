Option Strict On
Option Explicit On


Imports System.Data.SqlClient

Friend Class FormBox

    Inherits System.Windows.Forms.Form

    Public FacilityManagementIni As String
    Public mPathExes As String
    Public mPathAcces As String
    Public mExtFileAcces As String = ".acces"
    Public mExtFileExes As String = ".exe"
    Public mExtFileInfo As String = ".txt"
    Public mExtFileCRC As String = ".crc"
    Public mExtFileTmp As String = ".tmp"
    Public MenuPays As String = ""
    Public MenuSoft As String = ""
    Public FilterSupp As Boolean = False
    Public bUseShellExecute As Boolean = False
    Public bAdvancedMenu As Boolean = False
    Public bInsufficientDiskSpace As Boolean = False

    Public ListAccesClient_mDown As Boolean = False
    Public ListSelectClient_mDown As Boolean = False
    Public ListSelectPatch_mDown As Boolean = False
    Public ListPatch_mDown As Boolean = False

    Public Shared BaliseAccesEndOfFile As String = "##"

    Public Shared BaliseCRCAccesFile As String = "%CRC%"

    Public Shared BaliseUpdateEndOfFile As String = "##"

    Public Skip As String = "[SKIP]"
    Public Adm As String = "[ADM]"

    Public svAppBuild As String = Nothing
    Public ISPROJECTDIR As String = Nothing
    Public DATAFRET As String = Nothing
    Public DATA As String = Nothing

    Dim ListAcces() As String
    Public DicExes As New Dictionary(Of String, Boolean)
    Public DicPaysImage As New Dictionary(Of String, Image)
    Public DicSoftImage As New Dictionary(Of String, Image)
    Public CountryDic As New Dictionary(Of String, String)
    Public SoftwareDic As New Dictionary(Of String, String)
    Public cMSDic As New Dictionary(Of String, Integer)
    Public AliasDic As New Dictionary(Of String, String)

    Public nvOpacity As Double

    Public backcolor1 As Color = Color.LightBlue 'Fond
    Public backcolor2 As Color = Color.AliceBlue 'Text
    Public backcolor3 As Color = Color.SteelBlue 'Bandeau

    Dim SQL_Serveur As String ' = "172.17.230.209"
    Dim SQL_Login As String ' = "client"
    Dim SQL_Password As String ' = "client"
    Dim SQL_NoErrorMessage As Boolean = False
    Dim SQL_SiteInternet_Serveur As String = "172.17.230.209"
    ReadOnly SQL_SiteInternet_Login As String = "jfcupdate"
    ReadOnly SQL_SiteInternet_Password As String = "P@ssword02"

    Friend WithEvents SoftwareMenu As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents ExitMenu As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents CountryMenu As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents CustomerMenu As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents ToolsMenu As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents UpdateMenu As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents UnderMenuService As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents AddMenuService As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents DelMenuService As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents SearchMenuService As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents Group As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents Customer As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents AliasCustomer As System.Windows.Forms.ToolStripMenuItem

    Structure PATCH_INFO

        Dim count As String 'Integer
        Dim patch As String
        Dim size As String
        Dim crc As String
        Dim info As String

        Public Overrides Function ToString() As String
            Return count + "," + patch + "," + size + "," + crc + "," + info
        End Function

        Public Function FindString(ByVal stab() As String) As String
            For Each s In stab
                If s = BaliseAccesEndOfFile Or s = Nothing Then Exit For
                Dim sPathInfo() As String = Split(s, ",")
                If sPathInfo(1) = patch Then
                    Dim Index() As String = s.Split(CChar(","))
                    Return Index(0).Trim
                End If
            Next
            Return Nothing
        End Function

    End Structure

    Structure MAP_DRIVE
        Dim LETTER As String
        Dim PATH As String
        Dim TEST As String

        Public Function Value(ByVal svString As String) As Boolean
            Dim svSplit() As String = Split(svString, ",")
            If svSplit.Length = 3 Then
                LETTER = svSplit(0)
                PATH = svSplit(1)
                TEST = svSplit(2)
                Return True
            Else
                LETTER = Nothing
                PATH = Nothing
                TEST = Nothing
                Return False
            End If
        End Function

    End Structure

    Private Function AliasCompany(ByVal Name As String) As String

        Dim svString As String = Select_GetIniString("Alias", Name, FacilityManagementIni).Trim

        If svString = Nothing Then svString = Name

        AliasDic.Add(svString, Name)

        Return svString

    End Function

    Public Sub GetCustomerOfCountry(Optional ByVal F5 As Boolean = False)

        ListAccesClient.Items.Clear()
        ListSelectClient.Items.Clear()
        Label1.Text = "Customer List:"

        If IO.Directory.Exists(mPathAcces) Then

            ListAcces = IO.Directory.GetFiles(mPathAcces, "*" & mExtFileAcces, IO.SearchOption.TopDirectoryOnly)

            ListAcces = (From x In ListAcces Select x Order By x Ascending).ToArray
            ListAcces = (From x In ListAcces Select ParsePath(x, FILENAME_ONLY)).ToArray()

            AliasDic.Clear()

            For Each obj In ListAcces
                ListAccesClient.Items.Add(AliasCompany(obj))
            Next

            If Not F5 Then DelegatCheckClients()

            Label1.Text = "Customer List (" + CStr(ListAccesClient.Items.Count) + "):"
            Label2.Text = "Selected customers (" + CStr(ListSelectClient.Items.Count) + "):"

        End If

    End Sub

    Public Sub DelegatCheckClients()

        Dim DelGetFile As Action
        DelGetFile = New Action(AddressOf CheckClients)
        Dim ResGetFile = DelGetFile.BeginInvoke(Nothing, Nothing)

    End Sub

    Private Function ConvPays() As String

        'If MenuSoft = "Atelier Radio" Then

        '    If MenuPays = "French" Then
        '        Return "France"
        '    ElseIf MenuPays = "Poland" Then
        '        Return "Pologne"
        '    ElseIf MenuPays = "Russia" Then
        '        Return "Russie"
        '    ElseIf MenuPays = "Morocco" Then
        '        Return "Maroc"
        '    End If

        'End If

        Return MenuPays

    End Function

    Public Sub CreateSocieteAuto(Optional ByVal svDuplicateClient As String = Nothing)

        Dim DicSocieteCreatingAuto As Dictionary(Of String, String) = ReadSqlCreate()

        If DicSocieteCreatingAuto.Count = 0 Then Exit Sub

        Dim nResultAuto As Integer = MsgBoxResult.Cancel

        For Each svClient As String In DicSocieteCreatingAuto.Keys

            If StrComp(DicSocieteCreatingAuto(svClient), MenuPays, CompareMethod.Text) = 0 Then

                If AliasDic.ContainsValue(svClient) = False Then

                    Dim nResult As Integer

                    If nResultAuto <> MsgBoxResult.Yes Then
                        nResult = MsgBox("New Client: " + Chr(34) + svClient + Chr(34) + " ?", CType(MsgBoxStyle.Question + MsgBoxStyle.YesNoCancel, MsgBoxStyle), "Facility Management - " & MenuSoft & " - " & MenuPays)
                        'Dim nResult As Integer = MsgBox("New Client: " + Chr(34) + svClient + Chr(34) + " ?", CType(MsgBoxStyle.Question + MsgBoxStyle.YesNoCancel, MsgBoxStyle), "Facility Management - " & MenuSoft & " - " & MenuPays)
                    End If

                    If nResult = MsgBoxResult.Yes Then

                        If DicSocieteCreatingAuto.Count > 1 And nResultAuto = MsgBoxResult.Cancel Then
                            nResultAuto = MsgBox("Voulez-vous faire de même pour les autres clients ?", CType(MsgBoxStyle.Question + MsgBoxStyle.YesNo, MsgBoxStyle), "Facility Management - " & MenuSoft & " - " & MenuPays)
                        End If


                        If svDuplicateClient <> Nothing Then
                            Dim destFileAcces As String = mPathAcces + svClient + mExtFileAcces
                            Dim sourceFileAcces As String = mPathAcces + svDuplicateClient + mExtFileAcces
                            IO.File.Copy(sourceFileAcces, destFileAcces, True)

                            Dim CreationTime As Date = DateAndTime.Now

                            IO.File.SetCreationTime(destFileAcces, CreationTime)
                            IO.File.SetLastAccessTime(destFileAcces, CreationTime)
                            IO.File.SetLastWriteTime(destFileAcces, CreationTime)

                        Else
                            Dim svFileAcces As String = mPathAcces + svClient + mExtFileAcces
                            If IO.File.Exists(svFileAcces) Then IO.File.Delete(svFileAcces)
                            IO.File.WriteAllText(svFileAcces, BaliseAccesEndOfFile, System.Text.Encoding.Default)
                        End If


                        ListAccesClient.Items.Add(AliasCompany(svClient))

                    ElseIf nResult = MsgBoxResult.No Then
                        nResultAuto = MsgBoxResult.No

                    ElseIf nResult = MsgBoxResult.Cancel Then
                        Exit Sub

                    End If

                End If

            End If
        Next

    End Sub

    Public Sub CheckClients()

        Dim DicSocietePays As Dictionary(Of String, String) = ReadSql()

        If DicSocietePays.Count = 0 Then Exit Sub


        Dim szMessage As String = Nothing


        For Each obj In ListAccesClient.Items

            If DicSocietePays.Keys.Contains(AliasDic(obj.ToString)) = False And Not obj.ToString.Contains("JFC Test") Then

                If szMessage <> Nothing Then
                    szMessage = szMessage + vbNewLine + "- " + obj.ToString
                Else
                    szMessage = "- " + obj.ToString
                End If

            End If

        Next

        If szMessage <> Nothing Then
            szMessage = "List of customers not created in the database of serial numbers:" + vbNewLine + szMessage
        End If

        Dim szMessageBase As String = Nothing

        For Each a As String In DicSocietePays.Keys
            If DicSocietePays(a) = MenuPays Then

                If ListAcces.Contains(a) = False Then

                    If szMessageBase <> Nothing Then
                        szMessageBase = szMessageBase + vbNewLine + "- " + a
                    Else
                        szMessageBase = "- " + a
                    End If

                End If

            End If
        Next

        If szMessageBase <> Nothing Then
            szMessageBase = "List of customers not created in the access files:" + vbNewLine + szMessageBase
            If szMessage <> Nothing Then
                szMessage = szMessage + vbNewLine + vbNewLine + szMessageBase
            Else
                szMessage = szMessageBase
            End If
        End If

        If szMessage <> Nothing Then
            MsgBox(szMessage, CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - " & MenuSoft & " - " & MenuPays)
        End If

    End Sub

    Private Sub InitImage()

        CustomerMenu.Image = Facility_Management.My.Resources.utilisateurs
        ExitMenu.Image = Facility_Management.My.Resources.sortie
        CountryMenu.Image = Facility_Management.My.Resources.avertissement
        SoftwareMenu.Image = Facility_Management.My.Resources.disquedur
        UpdateMenu.Image = Facility_Management.My.Resources.Maj

        DicPaysImage.Clear()

        For Each pays In CountryDic.Keys

            Dim pathimage As String = ParsePath(Application.ExecutablePath, PATH) + "\Ressources\" + pays + ".png"

            If IO.File.Exists(pathimage) Then
                Dim bm As New Bitmap(pathimage)
                DicPaysImage.Add(pays, CType(bm, Image))
            Else
                DicPaysImage.Add(pays, Facility_Management.My.Resources.avertissement)
            End If

        Next

        DicSoftImage.Clear()

        For Each soft In SoftwareDic.Keys

            Dim pathimage As String = ParsePath(Application.ExecutablePath, PATH) + "\Ressources\" + soft + ".ico"

            If IO.File.Exists(pathimage) Then
                'Dim bm As New Bitmap(pathimage)
                Dim bm As New Icon(pathimage, New Size(32, 32))
                DicSoftImage.Add(soft, CType(bm.ToBitmap, Image))
            Else
                DicSoftImage.Add(soft, Facility_Management.My.Resources.avertissement)
            End If

        Next

        CountryMenu.DisplayStyle = ToolStripItemDisplayStyle.ImageAndText
        SoftwareMenu.DisplayStyle = ToolStripItemDisplayStyle.ImageAndText

    End Sub

    Private Sub InitMenu()

        Dim IndexCountry As Integer = 1
        Dim Country() As String = Split(Select_GetIniString("Parametres", "Country", FacilityManagementIni), ",")

        CountryDic.Clear()
        SoftwareDic.Clear()

        For a = 0 To Country.Length - 1 Step 2
            CountryDic.Add(Country(a).Trim, Country(a + 1).Trim + "\")
        Next

        Dim Software() As String = (From x As String In Split(Select_GetIniString("Parametres", "Software", FacilityManagementIni), ",") Select x = x.Trim Order By x Ascending).ToArray()
        Dim SoftAdvanced As String() = {"Medial 2", "AutoUpdate", "Facility Management", "MultiUser", "Services"}

        For Each NameSoft As String In Software
            If Not SoftAdvanced.Contains(NameSoft) Or bAdvancedMenu Then
                SoftwareDic.Add(NameSoft, Select_GetIniString(NameSoft, "PathExes", FacilityManagementIni).Trim)
            End If
        Next

        ListAccesClient.Sorted = True
        ListSelectClient.Sorted = True

        Me.SoftwareMenu = New System.Windows.Forms.ToolStripMenuItem
        Me.CountryMenu = New System.Windows.Forms.ToolStripMenuItem
        Me.CustomerMenu = New System.Windows.Forms.ToolStripMenuItem
        Me.UpdateMenu = New System.Windows.Forms.ToolStripMenuItem
        Me.ToolsMenu = New System.Windows.Forms.ToolStripMenuItem
        Me.ExitMenu = New System.Windows.Forms.ToolStripMenuItem
        Me.UnderMenuService = New System.Windows.Forms.ToolStripMenuItem
        Me.AddMenuService = New System.Windows.Forms.ToolStripMenuItem
        Me.DelMenuService = New System.Windows.Forms.ToolStripMenuItem
        Me.SearchMenuService = New System.Windows.Forms.ToolStripMenuItem

        Me.MenuStripFM.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.SoftwareMenu, Me.CountryMenu, Me.CustomerMenu, Me.UpdateMenu, Me.ToolsMenu, Me.ExitMenu})

        cMSDic.Clear()

        cMSDic.Add("New/Created", 0)
        cMSDic.Add("Edit selection", 1)
        cMSDic.Add("Delete selection", 2)
        cMSDic.Add("Add selection", 3)
        cMSDic.Add("Add all", 4)
        cMSDic.Add("Remove selection", 5)
        cMSDic.Add("Remove all selection", 6)
        cMSDic.Add("Search in customer", 7)
        cMSDic.Add("Duplicate customer", 8)
        cMSDic.Add("Add patchs from customers", 9)
        cMSDic.Add("Send FTP", 10)
        cMSDic.Add("Get data country", 11)
        cMSDic.Add("Create patch", 12)
        cMSDic.Add("Vide 13", 13)
        cMSDic.Add("Add all patchs from customer", 14)
        cMSDic.Add("Erase all data country", 15)
        cMSDic.Add("16", 16)
        cMSDic.Add("17", 17)
        cMSDic.Add("Send patchs automatically (Pige)", 18)
        cMSDic.Add("Refresh", 19)
        cMSDic.Add("Select all customer Pige", 20)
        cMSDic.Add("Invert selection", 21)
        cMSDic.Add("Vide 22", 22)
        cMSDic.Add("Vide 23", 23)
        cMSDic.Add("Vide 24", 24)
        cMSDic.Add("Vide 25", 25)
        cMSDic.Add("Vide 26", 26)
        cMSDic.Add("Vide 27", 27)
        cMSDic.Add("Vide 28", 28)
        cMSDic.Add("Vide 29", 29)
        cMSDic.Add("Vide 30", 30)
        cMSDic.Add("Vide 31", 31)
        cMSDic.Add("Vide 32", 32)
        cMSDic.Add("Add Group", 33)
        cMSDic.Add("Alias", 34)
        cMSDic.Add("Search in database", 35)
        cMSDic.Add("Vide 36", 36)
        cMSDic.Add("Vide 37", 37)
        cMSDic.Add("Vide 38", 38)
        cMSDic.Add("Vide 39", 39)
        cMSDic.Add("New/Created (Auto)", 40)
        cMSDic.Add("Import", 41)
        cMSDic.Add("Couper", 42)
        cMSDic.Add("Add patchs from customers (Send)", 43)
        cMSDic.Add("Vide 44", 44)
        cMSDic.Add("Vide 45", 45)
        cMSDic.Add("Vide 46", 46)
        cMSDic.Add("Vide 47", 47)
        cMSDic.Add("Rename", 48)
        cMSDic.Add("Skip patch from customers", 49)
        cMSDic.Add("Not skip patch from customers", 50)
        cMSDic.Add("Vide 51", 51)
        cMSDic.Add("Admin patch from customers", 52)
        cMSDic.Add("Not admin patch from customers", 53)

        CMS_ListAccesClient.Items.Add("New")
        CMS_ListPatch.Items.Add("New")
        CMS_ListSelectClient.Items.Add("New")
        CMS_ListSelectPatch.Items.Add("New")

        ToolStripStatusLabelFM.Text = ""

        Me.MenuStripFM.Size = New System.Drawing.Size(Me.Size.Width - 16, 40)

        CountryMenu.Text = "Country"
        CountryMenu.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        CountryMenu.ImageScaling = ToolStripItemImageScaling.None
        CountryMenu.AutoSize = False

        SoftwareMenu.Text = "Software"
        SoftwareMenu.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        SoftwareMenu.ImageScaling = ToolStripItemImageScaling.None
        SoftwareMenu.AutoSize = False

        CustomerMenu.Text = "Customer"
        CustomerMenu.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        CustomerMenu.ImageScaling = ToolStripItemImageScaling.None
        CustomerMenu.AutoSize = False

        UpdateMenu.Text = "Patch"
        UpdateMenu.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        UpdateMenu.ImageScaling = ToolStripItemImageScaling.None
        UpdateMenu.AutoSize = False

        ToolsMenu.Text = "Tools"
        ToolsMenu.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ToolsMenu.ImageScaling = ToolStripItemImageScaling.None
        ToolsMenu.AutoSize = False

        ExitMenu.Text = "Exit"
        ExitMenu.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ExitMenu.ImageScaling = ToolStripItemImageScaling.None
        ExitMenu.AutoSize = False

        FormBoxResize()

        'Warning Décalage si add new
        'CustomerMenu.DropDownItems.Count - 1
        'CustomerMenu.DropDownItems.Item(5).Visible
        'CustomerMenu.DropDownItems.Item(6).Visible

        CustomerMenu.DropDownItems.Add(cMSDic.Keys(9))
        CustomerMenu.DropDownItems.Item(CustomerMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        CustomerMenu.DropDownItems.Item(CustomerMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.telecharger

        CustomerMenu.DropDownItems.Add(cMSDic.Keys(43))
        CustomerMenu.DropDownItems.Item(CustomerMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        CustomerMenu.DropDownItems.Item(CustomerMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.telecharger
        CustomerMenu.DropDownItems.Item(CustomerMenu.DropDownItems.Count - 1).Visible = bAdvancedMenu

        CustomerMenu.DropDownItems.Add(cMSDic.Keys(31))
        CustomerMenu.DropDownItems.Item(CustomerMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        CustomerMenu.DropDownItems.Item(CustomerMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.conversion
        CustomerMenu.DropDownItems.Item(CustomerMenu.DropDownItems.Count - 1).Visible = bAdvancedMenu

        CustomerMenu.DropDownItems.Add(cMSDic.Keys(32))
        CustomerMenu.DropDownItems.Item(CustomerMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        CustomerMenu.DropDownItems.Item(CustomerMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.index
        CustomerMenu.DropDownItems.Item(CustomerMenu.DropDownItems.Count - 1).Visible = bAdvancedMenu

        UpdateMenu.DropDownItems.Add(cMSDic.Keys(10))
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.ftp

        UpdateMenu.DropDownItems.Add(cMSDic.Keys(44))
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.conversion
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Visible = bAdvancedMenu

        UpdateMenu.DropDownItems.Add(cMSDic.Keys(49))
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).ForeColor = Color.Green
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.dernier

        UpdateMenu.DropDownItems.Add(cMSDic.Keys(50))
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).ForeColor = Color.Green
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.premier

        UpdateMenu.DropDownItems.Add(cMSDic.Keys(52))
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).ForeColor = Color.Red
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.dernier

        UpdateMenu.DropDownItems.Add(cMSDic.Keys(53))
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).ForeColor = Color.Red
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.premier

        UpdateMenu.DropDownItems.Add(cMSDic.Keys(65))
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).ForeColor = Color.Purple
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.import
        UpdateMenu.DropDownItems.Item(UpdateMenu.DropDownItems.Count - 1).Visible = bAdvancedMenu

        ToolsMenu.Image = Facility_Management.My.Resources.tools

        ToolsMenu.DropDownItems.Add(cMSDic.Keys(19))
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.reload
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Name = cMSDic.Keys(19)

        ToolsMenu.DropDownItems.Add(cMSDic.Keys(12))
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Package2
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Name = cMSDic.Keys(12)

        ToolsMenu.DropDownItems.Add(cMSDic.Keys(11))
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.ImportData
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Name = cMSDic.Keys(11)

        ToolsMenu.DropDownItems.Add(cMSDic.Keys(18))
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Wizard_Wand
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Name = cMSDic.Keys(18)

        ToolsMenu.DropDownItems.Add(cMSDic.Keys(15))
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.corbeille
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Name = cMSDic.Keys(15)

        'Pige
        ToolsMenu.DropDownItems.Add(cMSDic.Keys(20))
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Wizard_Wand
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Name = cMSDic.Keys(20)

        'Invert
        ToolsMenu.DropDownItems.Add(cMSDic.Keys(21))
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources._Auto
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Name = cMSDic.Keys(21)

        'TaskList
        ToolsMenu.DropDownItems.Add(cMSDic.Keys(45))
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.radar_wifi_icone
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Visible = bAdvancedMenu
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Name = cMSDic.Keys(45)

        'Session
        ToolsMenu.DropDownItems.Add(cMSDic.Keys(46))
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.personne
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Visible = bAdvancedMenu
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Name = cMSDic.Keys(46)

        'TaskKill
        ToolsMenu.DropDownItems.Add(cMSDic.Keys(47))
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Delete
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Visible = bAdvancedMenu
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.Count - 1).Name = cMSDic.Keys(47)

        'Medial %REQUESTINFO%

        Me.CustomerMenu.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.UnderMenuService})

        UnderMenuService.Text = cMSDic.Keys(25)
        UnderMenuService.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        UnderMenuService.ImageScaling = ToolStripItemImageScaling.None
        UnderMenuService.Image = Facility_Management.My.Resources.tools
        UnderMenuService.AutoSize = True
        UnderMenuService.Name = cMSDic.Keys(25)

        Me.UnderMenuService.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.AddMenuService, Me.DelMenuService, Me.SearchMenuService})

        AddMenuService.Text = cMSDic.Keys(3)
        AddMenuService.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        AddMenuService.ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.Image = Facility_Management.My.Resources.Add
        AddMenuService.AutoSize = True
        AddMenuService.Name = cMSDic.Keys(3)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(22)) 'Option
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Info
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(22)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(23))
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Services
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(23)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(24)) 'Option
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.zip2
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(24)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(26)) 'Option
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.system
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(26)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(27)) 'Option
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.xml
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(27)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(28)) 'Option
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Supervisor
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(28)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(29)) 'Option
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.status
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(29)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(30))
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.graphique
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(30)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(4))
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Ajouter
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(4)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(36))
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.document_export
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(36)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(37))
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.document_noexport
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(37)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(38))
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.wait
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(38)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(39))
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.utilisateur
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(39)

        AddMenuService.DropDownItems.Add(cMSDic.Keys(51))
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.globe1
        AddMenuService.DropDownItems.Item(AddMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(51)

        DelMenuService.Text = cMSDic.Keys(2)
        DelMenuService.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        DelMenuService.ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.Image = Facility_Management.My.Resources.Delete
        DelMenuService.AutoSize = True
        DelMenuService.Name = cMSDic.Keys(2)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(22)) 'Option
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Info
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(22)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(23))
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Services
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(23)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(24)) 'Option
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.zip2
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(24)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(26)) 'Option
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.system
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(26)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(27)) 'Option
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.xml
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(27)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(28)) 'Option
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Supervisor
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(28)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(29)) 'Option
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.status
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(29)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(30))
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.graphique
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(30)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(6))
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Services
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(6)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(36))
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.document_export
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(36)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(37))
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.document_noexport
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(37)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(38))
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.wait
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(38)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(39))
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.utilisateur
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(39)

        DelMenuService.DropDownItems.Add(cMSDic.Keys(51))
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.globe1
        DelMenuService.DropDownItems.Item(DelMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(51)

        SearchMenuService.Text = cMSDic.Keys(7)
        SearchMenuService.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        SearchMenuService.ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.Image = Facility_Management.My.Resources.Preview
        SearchMenuService.AutoSize = True
        SearchMenuService.Name = cMSDic.Keys(7)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(22)) 'Option
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Info
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(22)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(23))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Services
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(23)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(24)) 'Option
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.zip2
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(24)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(26)) 'Option
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.system
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(26)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(27)) 'Option
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.xml
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(27)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(28)) 'Option
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Supervisor
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(28)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(29)) 'Option
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.status
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(29)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(30))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.graphique
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(30)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(7))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.find
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(7)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(36))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.document_export
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(36)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(37))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.document_noexport
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(37)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(38))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.wait
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(38)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(39))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.utilisateur
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(39)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(51))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.globe1
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(51)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(59))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.wait
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(59)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(60))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.graphique
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(60)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(61))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.graphique
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(61)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(63))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.Maj
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(63)

        SearchMenuService.DropDownItems.Add(cMSDic.Keys(64))
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Image = Facility_Management.My.Resources.gnome
        SearchMenuService.DropDownItems.Item(SearchMenuService.DropDownItems.Count - 1).Name = cMSDic.Keys(64)


        CMS_ListAccesClient.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        CMS_ListPatch.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        CMS_ListSelectClient.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        CMS_ListSelectPatch.Font = New System.Drawing.Font("Tahoma", 14.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))

        CountryMenu.BackColor = backcolor3
        SoftwareMenu.BackColor = backcolor3
        ExitMenu.BackColor = backcolor3
        UpdateMenu.BackColor = backcolor3
        ToolsMenu.BackColor = backcolor3
        CustomerMenu.BackColor = backcolor3
        UnderMenuService.BackColor = backcolor3
        CustomerMenu.BackColor = backcolor3

        CMS_ListAccesClient.BackColor = backcolor1
        CMS_ListPatch.BackColor = backcolor1
        CMS_ListSelectClient.BackColor = backcolor1
        CMS_ListSelectPatch.BackColor = backcolor1

        'ToolsMenu.BackColor = backcolor3
        For index As Integer = 0 To ToolsMenu.DropDownItems.Count - 1
            ToolsMenu.DropDownItems.Item(index).BackColor = backcolor2
        Next

        'UpdateMenu.BackColor = backcolor3
        For index As Integer = 0 To UpdateMenu.DropDownItems.Count - 1
            UpdateMenu.DropDownItems.Item(index).BackColor = backcolor2
        Next

        'CustomerMenu.BackColor = backcolor3
        For index As Integer = 0 To CustomerMenu.DropDownItems.Count - 1
            CustomerMenu.DropDownItems.Item(index).BackColor = backcolor2
        Next

        AddMenuService.BackColor = backcolor2
        For index As Integer = 0 To AddMenuService.DropDownItems.Count - 1
            AddMenuService.DropDownItems.Item(index).BackColor = backcolor2
        Next

        DelMenuService.BackColor = backcolor2
        For index As Integer = 0 To DelMenuService.DropDownItems.Count - 1
            DelMenuService.DropDownItems.Item(index).BackColor = backcolor2
        Next

        SearchMenuService.BackColor = backcolor2
        For index As Integer = 0 To SearchMenuService.DropDownItems.Count - 1
            SearchMenuService.DropDownItems.Item(index).BackColor = backcolor2
        Next

    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        'Controle si une instance existe déjà 
        If Diagnostics.Process.GetProcessesByName(Diagnostics.Process.GetCurrentProcess.ProcessName).Length > 1 And Not CBool(Select_GetIniString("Parametres", "AdvancedMenu", ParsePath(Application.ExecutablePath, PATH_FILENAME_ONLY) & ".ini") = "1") Then
            Dim NumberOfProcess As Integer = 0
            Dim PathProcessCurrent As String = Diagnostics.Process.GetCurrentProcess.MainModule.FileName ' Chemin complet de l'application
            Dim NameProcessCurrent As String = Diagnostics.Process.GetCurrentProcess.ProcessName.ToUpper ' Nom application
            'Dim NameProcessCurrent As String = Diagnostics.Process.GetCurrentProcess.MainModule.ModuleName 'Nom de l'exe

            Dim Prc() As Process = Process.GetProcesses() 'on récupère la liste des processus de Windows: ATTENTION ce sont des tableaux
            For x As Integer = 0 To UBound(Prc) ' on fait toute la liste dans une boucle
                'Dim pName As String = Prc(x).ProcessName.ToUpper(New System.Globalization.CultureInfo("fr-FR", False))
                Dim pName As String = Prc(x).ProcessName.ToUpper(New System.Globalization.CultureInfo("en-US", False))
                If pName = NameProcessCurrent Then ' Cette application nous embete ?

                    Dim pPathProcess As String = Nothing

                    'Access is denied (Multi session)
                    Try
                        pPathProcess = Prc(x).MainModule.FileName
                    Catch ex As Exception
                        'MsgBox(ex.Message, MsgBoxStyle.Critical)
                    End Try

                    If pPathProcess = PathProcessCurrent Then NumberOfProcess += 1
                    If NumberOfProcess > 1 Then
                        Dim szMesssage As String = "Application already open" + vbNewLine + vbNewLine + "Do you really want to continue?"
                        If MsgBox(szMesssage, CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle), My.Application.Info.Title) = MsgBoxResult.No Then
                            'ShowWindow(CInt(Prc(x).Handle), SW_SHOW)
                            Me.Close()
                        End If
                    End If
                End If
            Next
        End If

        Me.BackColor = backcolor1
        Me.MenuStripFM.BackColor = backcolor1
        Me.StatusStripFM.BackColor = backcolor1
        'Me.Label_Shortcut.BackColor = backcolor1
        Me.F2.BackColor = backcolor3
        Me.F3.BackColor = backcolor3
        Me.F4.BackColor = backcolor3
        Me.F5.BackColor = backcolor3
        Me.F6.BackColor = backcolor3
        Me.F9.BackColor = backcolor3
        Me.F10.BackColor = backcolor3

        Me.ButtonFiltreUpdates.BackColor = backcolor3
        Me.Label_version.BackColor = backcolor2
        Me.FiltrePatch.BackColor = backcolor2
        Me.ListSelectPatch.BackColor = backcolor2
        Me.ListSelectClient.BackColor = backcolor2
        Me.ListPatch.BackColor = backcolor2
        Me.ListAccesClient.BackColor = backcolor2

        CreateUpdate.BackColor = backcolor1
        CreateUpdate.TextPatchInformation.BackColor = backcolor2
        CreateUpdate.ComboBoxPatchName.BackColor = backcolor2
        CreateUpdate.ButtonExit.BackColor = backcolor3
        CreateUpdate.ButtonCreating.BackColor = backcolor3
        CreateUpdate.StatusStrip1.BackColor = backcolor1
        CreateUpdate.ButtonGetData.BackColor = backcolor3
        CreateUpdate.ButtonGetVersion.BackColor = backcolor3
        CreateUpdate.ButtonGetTarifs.BackColor = backcolor3
        CreateUpdate.ButtonPigeAuto.BackColor = backcolor3

        DataRecoveryP.BackColor = backcolor1
        DataRecoveryP.ButtonExit.BackColor = backcolor3
        DataRecoveryP.ButtonCreating.BackColor = backcolor3
        DataRecoveryP.ComboBox1.BackColor = backcolor2
        DataRecoveryP.TreeViewList.BackColor = backcolor2
        DataRecoveryP.TreeViewSelected.BackColor = backcolor2
        DataRecoveryP.ToolStripProgressBar1.BackColor = backcolor2
        DataRecoveryP.StatusStrip1.BackColor = backcolor1

        DataRecovery.BackColor = backcolor1
        DataRecovery.ButtonExit.BackColor = backcolor3
        DataRecovery.ButtonCreating.BackColor = backcolor3
        DataRecovery.ButtonZip.BackColor = backcolor3
        DataRecovery.ButtonUnZip.BackColor = backcolor3
        DataRecovery.ButtonParcourir.BackColor = backcolor3
        DataRecovery.TreeViewSelected.BackColor = backcolor2
        DataRecovery.TreeViewList.BackColor = backcolor2
        DataRecovery.ToolStripProgressBar2.BackColor = backcolor2
        DataRecovery.ComboBox1.BackColor = backcolor2
        DataRecovery.StatusStrip1.BackColor = backcolor1

        'ProgressBarPleaseWait
        PleaseWait.BackColor = backcolor1

        GetElements.BackColor = backcolor1
        GetElements.ComboBoxVersion.BackColor = backcolor2
        GetElements.ButtonIgnore.BackColor = backcolor3
        GetElements.ButtonCancel.BackColor = backcolor3
        GetElements.ButtonGet.BackColor = backcolor3

        'MsgBox(My.Computer.FileSystem.SpecialDirectories.Temp)

        ' Définition d'un chemin de fichier pour d'autre extension
        Dim FacilityManagementPath As String = ParsePath(Application.ExecutablePath, PATH_FILENAME_ONLY)

        ' Fichier INI de paramétrages
        FacilityManagementIni = FacilityManagementPath & ".ini"
        If IO.File.Exists(FacilityManagementIni) = False Then
            MsgBox("Missing configuration file", MsgBoxStyle.Critical, My.Application.Info.Title)
            End
        End If

        ' *******************************************************************
        ' *****    Taille du répertoire TEMP de la VM KMCHMD5PWR4W2     *****
        ' *******************************************************************
        'Dim szFolder As String = Nothing

        'Dim szHostName As String = My.Computer.Name

        'Dim svComputerName As String = Select_GetIniString("Process", "ComputerName", FacilityManagementIni).Trim
        'If svComputerName = "" Then
        '    svComputerName = "KMCHMD5PWR4W2"
        'End If

        'If svComputerName <> Nothing Then

        '    If StrComp(szHostName, svComputerName, CompareMethod.Text) = 0 Then
        '        szFolder = IO.Path.GetTempPath() '"C:\WINDOWS\TEMP"
        '    Else
        '        'MsgBox("HostName: " + szHostName + vbNewLine + "ComputerName: " + svComputerName, MsgBoxStyle.Information)
        '        szFolder = "\\" + svComputerName + "\c$\WINDOWS\TEMP"
        '    End If

        '    Dim szDirSize As String = "0o"
        '    Dim szAvailableFreeSpace As String = "0o"

        '    szAvailableFreeSpace = ConvertOctets(GetFreeSpace(szFolder))

        '    Dim FolderInfo As New IO.DirectoryInfo(szFolder)

        '    Try
        '        'Dim dSecurity As DirectorySecurity = FolderInfo.GetAccessControl()
        '        szDirSize = ConvertOctets(DirSize(FolderInfo))
        '    Catch ex As Exception
        '        'MsgBox(ex.Message, vbCritical)
        '    End Try

        '    'ToolStripStatusLabelDisk.Text = "Update: " + szDirSize + " / FreeSpace: " + szAvailableFreeSpace

        '    ToolStripStatusLabelDisk.Text = "Temp: " + szDirSize + " / FreeSpace: " + szAvailableFreeSpace

        'Else
        ToolStripStatusLabelDisk.Text = ""
        'End If
        ' *******************************************************************


        F2.Text = "F2 new client"
        F3.Text = "F3 create patch"
        F4.Text = "F4 edit client"
        F5.Text = "F5 refresh"
        F6.Text = "F6 add/send patch"
        F9.Text = "F9 Search customer"
        F10.Text = "F10 Search database"

        Label_version.Text = "v" + Application.ProductVersion.ToString()

        Dim AvailableFreeSpace As Double = My.Computer.FileSystem.GetDriveInfo(FacilityManagementPath).AvailableFreeSpace

        Dim FreeSpace As Integer = 0

        Dim svFreeSpace As String = Select_GetIniString("Parametres", "FreeSpace", FacilityManagementIni).Trim
        If Not Int32.TryParse(svFreeSpace, FreeSpace) Then
            FreeSpace = 500
            Select_WriteIniString("Parametres", "FreeSpace", CStr(FreeSpace), FacilityManagementIni)
        End If

        If CDbl(FreeSpace) * 1048576 > AvailableFreeSpace Then ' en Mo
            MsgBox("Insufficient disk space. (Free space: " + ConvertOctets(AvailableFreeSpace) + ")", MsgBoxStyle.Critical, My.Application.Info.Title)
            bInsufficientDiskSpace = True
        ElseIf mConnection_GetLastManage() Then
            Process.Start(Application.ExecutablePath, "ReStart")
            Me.Close()
        End If

        PleaseWait.Show()
        System.Windows.Forms.Application.DoEvents()

        'MapDrive
        Dim svMapDrive As MAP_DRIVE = New MAP_DRIVE
        Dim nvCountMap As Integer = 0

        Do
            If svMapDrive.Value(Select_GetIniString("Parametres", "MapDrive" + CStr(nvCountMap).Trim, FacilityManagementIni)) = False Then Exit Do

            If Not IO.File.Exists(svMapDrive.TEST) Then
                UnMapDrive(svMapDrive.LETTER)
                MapDrive(svMapDrive.LETTER, svMapDrive.PATH)
                'MapDriveNetUse(svMapDrive.LETTER, svMapDrive.PATH)
            End If

            nvCountMap += 1

        Loop

        'Opacity
        nvOpacity = CDbl((Select_GetIniString("Parametres", "Opacity", FacilityManagementIni).Trim)) '0.95
        If nvOpacity = 0 Then nvOpacity = 100
        Me.Opacity = nvOpacity / 100 '0.95

        'InstallShield
        ISPROJECTDIR = Select_GetIniString("Parametres", "ISPROJECTDIR", FacilityManagementIni).Trim

        DATAFRET = Select_GetIniString("Parametres", "DATAFRET", FacilityManagementIni).Trim

        DATA = Select_GetIniString("Parametres", "DATA", FacilityManagementIni).Trim

        If bInsufficientDiskSpace Then
            MsgBox("Probleme d'espace disque !" + vbNewLine + vbNewLine + "La création de mise à jour sera impossible." + vbNewLine + vbNewLine + "Veuillez contacter le responsable.", MsgBoxStyle.Critical, My.Application.Info.Title)
            svAppBuild = Nothing
        Else
            svAppBuild = Select_GetIniString("Parametres", "InstallShield", FacilityManagementIni).Trim
        End If

        If IO.File.Exists(svAppBuild) Then

            Dim LicenseInstallShield = Select_GetIniString("Parametres", "LicenseInstallShield", FacilityManagementIni)
            Dim LicenselFound As String = MacAddress.GetValue(LicenseInstallShield)

            If StrComp(LicenseInstallShield, LicenselFound, CompareMethod.Text) <> 0 Then
                MsgBox("Probleme de licence Installshield !" + vbNewLine + vbNewLine + "La création de mise à jour sera impossible." + vbNewLine + vbNewLine + "Veuillez contacter le responsable.", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & LicenselFound)
                svAppBuild = Nothing
            End If

        Else
            svAppBuild = Nothing
        End If

        'UseShellExecute
        bUseShellExecute = (Select_GetIniString("Parametres", "UseShellExecute", FacilityManagementIni) = "1")

        'Serveur
        SQL_Serveur = Select_GetIniString("SQL", "Server", FacilityManagementIni).Trim
        SQL_Login = Select_GetIniString("SQL", "Login", FacilityManagementIni).Trim
        SQL_Password = Select_GetIniString("SQL", "Password", FacilityManagementIni).Trim

        SQL_NoErrorMessage = Not (Select_GetIniString("SQL", "NoErrorMessage", FacilityManagementIni).Trim = "0")

        SQL_SiteInternet_Serveur = SQL_Serveur

        InitMenu()

        InitImage()

        MenuSoft = Select_GetIniString("Parametres", "SoftwareDefault", FacilityManagementIni).Trim
        MenuPays = Select_GetIniString("Parametres", "CountryDefault", FacilityManagementIni).Trim

        If MenuSoft <> "" Then
            Init_Software()
        End If

        If MenuPays <> "" Then
            Init_Country()
        End If

        GetCustomerOfCountry()

        PleaseWait.Hide()

        Me.KeyPreview = True

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub ListPatch_MouseDoubleClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListPatch.MouseDoubleClick
        SelectedPatch()
    End Sub

    Private Sub ListSelectClient_MouseDoubleClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListSelectClient.MouseDoubleClick
        UnSelectedClient()
    End Sub

    Private Sub ListAccesClient_MouseDoubleClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListAccesClient.MouseDoubleClick
        SelectedClient()
    End Sub

    Private Sub ButtonUnAddAllClients_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        UnSelectedAllClient()
    End Sub

    Private Sub ButtonAddAllClients_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        SelectedAllClient()
    End Sub

    Private Sub ButtonUnAddAllPatchs_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        UnSelectedAllPatch()
    End Sub

    Private Sub ExitMenu_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ExitMenu.Click
        Me.Close()
    End Sub

    Private Sub ListSelectPatch_MouseDoubleClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListSelectPatch.MouseDoubleClick
        UnSelectedPatch()
    End Sub

    Private Sub CustomerMenu_DropDownItemClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles CustomerMenu.DropDownItemClicked

        Disabled_FormBox()

        Dim str As String = e.ClickedItem.Text

        Dim int As Integer = cMSDic(str)

        CustomerMenu.HideDropDown()

        Select Case int

            Case 9

                AddPatchToClient()

            Case 43

                AddPatchToClient(, , False)

                If MsgBox("Do you want to send the patches to the FTP server?", CType(MsgBoxStyle.Information + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle), "Facility Management - Transfer FTP Customers and patchs") = MsgBoxResult.Yes Then

                    Send2FTP(, , False)

                End If

                MsgBox("Completed.", CType(MsgBoxStyle.Information + MsgBoxStyle.OkOnly + MsgBoxStyle.SystemModal, MsgBoxStyle))

        End Select

        Enabled_FormBox()

    End Sub

    Private Sub UpdateMenu_DropDownItemClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles UpdateMenu.DropDownItemClicked

        Dim str As String = e.ClickedItem.Text

        Dim int As Integer = cMSDic(str)

        Select Case int

            Case 10 'FTP
                Send2FTP()

            Case 44 'FTP
                CRC2File()

            Case 49 'SKIP
                SkipPatchToClient()

            Case 50 'Not Skip
                NotSkipPatchToClient()

            Case 52 'ADM
                AdmPatchToClient()

            Case 53 'Not ADM
                NotAdmPatchToClient()

        End Select

    End Sub

    Private Sub ToolsMenu_DropDownItemClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles ToolsMenu.DropDownItemClicked


        Disabled_FormBox()

        Dim str As String = e.ClickedItem.Text

        Dim int As Integer = cMSDic(str)

        ToolsMenu.HideDropDown()

        Select Case int

            Case 11 'Get data
                If Select_GetIniString(MenuSoft, "DataRecovery", FacilityManagementIni).Trim = "P" Then
                    DataRecoveryP.ShowDialog()
                ElseIf Select_GetIniString(MenuSoft, "DataRecovery", FacilityManagementIni).Trim = "I" Then
                    DataRecovery.ShowDialog()
                End If

            Case 12 'Create patch

                CreateUpdate.ShowDialog()

            Case 15 'Erase data

                If MsgBox("Would you erase data from all countries", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton1, MsgBoxStyle), "Clear Data") = MsgBoxResult.Yes Then

                    Dim mDestination = Select_GetIniString(MenuSoft, "Destination", FacilityManagementIni).Trim
                    Dim CheckData() As String = IO.Directory.GetDirectories(mDestination)
                    For Each z In CheckData
                        IO.Directory.Delete(z, True)
                    Next

                    MsgBox("Completed.", CType(MsgBoxStyle.Information + MsgBoxStyle.SystemModal, MsgBoxStyle))

                End If

            Case 18 ' Send automatic

                If MsgBox("Are you sure you want to send patches automatically (Pige)?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle), "Send Automatic") = MsgBoxResult.Yes Then

                    UnSelectedAllClient()
                    UnSelectedAllPatch()

                    GetElements.ComboBoxVersion.Items.Clear()
                    GetElements.ComboBoxVersion.Text = ""
                    GetElements.Text = "Send Automatic"
                    GetElements.ChooseLabel.Text = "Select a patch to send:"
                    GetElements.ButtonGet.Text = "Send"

                    Dim bmp As System.Drawing.Bitmap = Facility_Management.My.Resources.ftp
                    Dim ico As System.Drawing.Icon = System.Drawing.Icon.FromHandle(bmp.GetHicon())
                    GetElements.Icon = ico

                    Dim PatchFind As String = Select_GetIniString(MenuSoft, "PatchFind", FacilityManagementIni).Trim

                    '--------------------------------------------------------------------------------------------------------------
                    'Passqge en ZIP à modifier
                    '--------------------------------------------------------------------------------------------------------------
                    Dim ListPatchPige() As String = IO.Directory.GetFiles(mPathExes, PatchFind)
                    ListPatchPige = (From w In ListPatchPige Select w Order By IO.File.GetCreationTime(w) Descending).ToArray()
                    ListPatchPige = (From w In ListPatchPige Select Replace(w, mPathExes, "")).ToArray()
                    ListPatchPige = (From w In ListPatchPige Select Replace(w, "All.exe", "")).ToArray()
                    '--------------------------------------------------------------------------------------------------------------

                    GetElements.ComboBoxVersion.Items.AddRange(ListPatchPige)

                    If GetElements.ComboBoxVersion.Items.Count > 0 Then
                        GetElements.ComboBoxVersion.Text = CStr(GetElements.ComboBoxVersion.Items.Item(0))
                    End If

                    If GetElements.ShowDialog() = Windows.Forms.DialogResult.OK Then
                        Dim svSelectedElement As String = GetElements.ComboBoxVersion.Text


                        'Ajout control Pige -1
                        Dim IndexOfSelecdedElement As Integer = Array.IndexOf(ListPatchPige.ToArray, svSelectedElement)
                        Dim PatchLessOne As String = Nothing
                        If (ListPatchPige.Length - 1) > IndexOfSelecdedElement Then
                            PatchLessOne = ListPatchPige(IndexOfSelecdedElement + 1)
                        End If


                        Dim PigeKey As String = Select_GetIniString(MenuSoft, "Key1", FacilityManagementIni).Trim

                        Dim i As Integer = 1

                        While PigeKey <> ""

                            Dim key() As String = Split(PigeKey, ",")

                            SelectedPatch(svSelectedElement + key(1) + mExtFileExes)

                            Dim j As Integer = 2
                            While j < key.Length

                                If AliasDic.ContainsKey(key(j)) Then
                                    Dim client As String = AliasDic(key(j))
                                    If IO.File.Exists(mPathAcces + client + mExtFileAcces) Then
                                        SelectedClient(key(j))
                                    End If
                                ElseIf IO.File.Exists(mPathAcces + key(j) + mExtFileAcces) Then
                                    SelectedClient(key(j))
                                End If

                                System.Windows.Forms.Application.DoEvents()
                                j += 1
                            End While

                            If ListSelectClient.Items.Count > 0 Then
                                AddPatchToClient(, , False, PatchLessOne + key(1))
                                Send2FTP(, , False)
                            End If

                            UnSelectedAllClient()
                            UnSelectedAllPatch()
                            System.Windows.Forms.Application.DoEvents()

                            i += 1
                            PigeKey = Select_GetIniString(MenuSoft, "Key" + Trim(CStr(i)), FacilityManagementIni).Trim

                        End While

                        MsgBox("Completed.", CType(MsgBoxStyle.Information + MsgBoxStyle.OkOnly + MsgBoxStyle.SystemModal, MsgBoxStyle))

                    End If

                End If

            Case 20 'Pige

                SelectedAllClientPige()

            Case 19 'Refresh

                Init_Software()
                GetCustomerOfCountry()

        End Select

        Enabled_FormBox()


    End Sub

    Private Sub CountryMenu_DropDownItemClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles CountryMenu.DropDownItemClicked

        MenuPays = e.ClickedItem.Text

        Select_WriteIniString("Parametres", "CountryDefault", MenuPays, FacilityManagementIni)

        Init_Country()

        GetCustomerOfCountry()

        'for each (ToolStripMenuItem tDecoch in cMS.Items) //cMS étant mon contextmenustrip 
        '    tDecoch.Checked = False //tout décocher
        'Next
        't.Checked = True //cocher l'item sur lequel j'ai cliqué 

    End Sub

    Private Sub Init_Country()

        If mPathExes <> "" Then
            mPathAcces = mPathExes + CountryDic(MenuPays)
        Else
            mPathAcces = ""
        End If

        If MenuSoft <> "" Then
            Me.Text = "Facility Management - " & MenuSoft & " - " & MenuPays
        Else
            Me.Text = "Facility Management - " & MenuPays
        End If

        CountryMenu.Image = DicPaysImage(MenuPays)
        CountryMenu.Text = MenuPays

    End Sub

    Private Sub SelectedAllClientPige()

        UnSelectedAllClient()
        UnSelectedAllPatch()


        Dim PigeKey As String = Select_GetIniString(MenuSoft, "Key1", FacilityManagementIni).Trim

        Dim i As Integer = 1

        While PigeKey <> ""

            Dim key() As String = Split(PigeKey, ",")

            Dim j As Integer = 2
            While j < key.Length


                If AliasDic.ContainsKey(key(j)) Then
                    Dim client As String = AliasDic(key(j))
                    If IO.File.Exists(mPathAcces + client + mExtFileAcces) Then
                        SelectedClient(key(j))
                    End If
                ElseIf IO.File.Exists(mPathAcces + key(j) + mExtFileAcces) Then
                    SelectedClient(key(j))
                End If


                System.Windows.Forms.Application.DoEvents()
                j += 1
            End While

            System.Windows.Forms.Application.DoEvents()

            i += 1
            PigeKey = Select_GetIniString(MenuSoft, "Key" + Trim(CStr(i)), FacilityManagementIni).Trim

        End While

    End Sub

    Private Sub Init_Software()

        If Not SoftwareDic.ContainsKey(MenuSoft) Then
            Exit Sub
        End If

        mPathExes = SoftwareDic(MenuSoft)

        If MenuPays <> "" Then
            Me.Text = "Facility Management - " & MenuSoft & " - " & MenuPays
            mPathAcces = mPathExes + CountryDic(MenuPays)
        Else
            Me.Text = "Facility Management - " & MenuSoft
            mPathAcces = ""
        End If

        CreateListExes()

        If FiltrePatch.Text <> Nothing Then
            PatchFilter()
        End If

        CountryMenu.DropDownItems.Clear()

        ' *****************************************
        'CountryMenu.CheckState = CheckState.Checked
        'CountryMenu.CheckOnClick = True
        'CountryMenu.
        ' *****************************************

        For Each u In CountryDic.Keys
            If IO.Directory.Exists(mPathExes + CountryDic(u)) Then
                CountryMenu.DropDownItems.Add(u, DicPaysImage(u))
                CountryMenu.DropDownItems.Item(CountryMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
                CountryMenu.DropDownItems.Item(CountryMenu.DropDownItems.Count - 1).BackColor = backcolor2
            End If
        Next

        ' Modif en cours (Fini)
        If CountryMenu.DropDownItems.Count = 0 Then
            CountryMenu.Image = Facility_Management.My.Resources.avertissement
        ElseIf CountryMenu.DropDownItems.Count = 1 Then
            MenuPays = CountryMenu.DropDownItems.Item(0).Text
            CountryMenu.DropDownItems.Clear()
            CountryMenu.Image = DicPaysImage(MenuPays)
            CountryMenu.Text = MenuPays
            Me.Text = "Facility Management - " & MenuSoft & " - " & MenuPays
            mPathAcces = mPathExes + CountryDic(MenuPays)
        End If

        SoftwareMenu.Image = DicSoftImage(MenuSoft)
        SoftwareMenu.Text = MenuSoft

        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.IndexOfKey(cMSDic.Keys(12))).Visible = Not (Select_GetIniString(MenuSoft, "CreatePatch", FacilityManagementIni).Trim = "0") 'True 'IO.File.Exists(svAppBuild)
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.IndexOfKey(cMSDic.Keys(11))).Visible = (Val(Select_GetIniString(MenuSoft, "ToolsMenuGetData", FacilityManagementIni).Trim) = 1)
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.IndexOfKey(cMSDic.Keys(18))).Visible = (Val(Select_GetIniString(MenuSoft, "ToolsMenuPige", FacilityManagementIni).Trim) = 1)
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.IndexOfKey(cMSDic.Keys(15))).Visible = (Val(Select_GetIniString(MenuSoft, "ToolsMenuEraseData", FacilityManagementIni).Trim) = 1)
        ToolsMenu.DropDownItems.Item(ToolsMenu.DropDownItems.IndexOfKey(cMSDic.Keys(20))).Visible = (Val(Select_GetIniString(MenuSoft, "ToolsMenuPige", FacilityManagementIni).Trim) = 1)

        CustomerMenu.DropDownItems.Item(CustomerMenu.DropDownItems.IndexOfKey(cMSDic.Keys(25))).Visible = bAdvancedMenu '(Val(Select_GetIniString(MenuSoft, "MenuService", FacilityManagementIni).Trim) = 1)

        Dim Medial_2 As Boolean = (StrComp(MenuSoft, "Medial 2", CompareMethod.Text) = 0)
        Dim Atelier_Radio As Boolean = (StrComp(MenuSoft, "Atelier Radio", CompareMethod.Text) = 0)

        Dim UpdateAppsMenu As Boolean = Not (Val(Select_GetIniString(MenuSoft, "UpdateApps", FacilityManagementIni).Trim) = 1)
        ListAccesClient.Visible = UpdateAppsMenu
        ListSelectClient.Visible = UpdateAppsMenu
        CustomerMenu.Enabled = UpdateAppsMenu
        CountryMenu.Enabled = UpdateAppsMenu
        Label2.Visible = UpdateAppsMenu
        ListSelectClient.Visible = UpdateAppsMenu
        Label1.Visible = UpdateAppsMenu
        ListAccesClient.Visible = UpdateAppsMenu
        ToolsMenu.Enabled = UpdateAppsMenu

        mConnection_initFTP(MenuSoft)

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub SoftwareMenu_DropDownItemClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles SoftwareMenu.DropDownItemClicked

        PleaseWait.Show()
        System.Windows.Forms.Application.DoEvents()

        MenuSoft = e.ClickedItem.Text

        Select_WriteIniString("Parametres", "SoftwareDefault", MenuSoft, FacilityManagementIni)

        Init_Software()

        GetCustomerOfCountry()

        PleaseWait.Hide()
        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub CMS_ListAccesClient_ItemClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles CMS_ListAccesClient.ItemClicked

        Disabled_FormBox()

        If mPathAcces <> Nothing Then

            Dim str As String = e.ClickedItem.Text

            Dim int As Integer = cMSDic(str)

            Dim client As String = CStr(ListAccesClient.SelectedItem)

            If client <> Nothing Then
                client = AliasDic(client)
            End If

            'Dim client As String = AliasDic(CStr(ListAccesClient.SelectedItem))

            CMS_ListAccesClient.Hide()

            Select Case int

                Case 0 'New
                    Dim NewClient As String = InputBox("New Client:", , , CMS_ListAccesClient.Bounds.X, CMS_ListAccesClient.Bounds.Y).Trim
                    If NewClient <> Nothing Then
                        NewClient.Trim()

                        AliasCompany(NewClient)

                        For Each u In CountryDic
                            Dim PathFileAcces As String = mPathExes + u.Value + NewClient + mExtFileAcces
                            If IO.File.Exists(PathFileAcces) Then
                                MsgBox("The client '" + NewClient + "' already exists in'" + u.Key + "'.", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - New Client")
                                Enabled_FormBox()
                                Exit Sub
                            End If
                        Next

                        Dim svFileAcces As String = mPathAcces + NewClient + mExtFileAcces
                        If IO.File.Exists(svFileAcces) Then
                            IO.File.Delete(svFileAcces)
                        End If

                        IO.File.WriteAllText(svFileAcces, AddCRC2String(BaliseAccesEndOfFile), System.Text.Encoding.Default)

                        ListAccesClient.Items.Add(NewClient)

                    End If

                Case 8 'Duplicate
                    Dim NewClient As String = InputBox("New Client:", , , CMS_ListAccesClient.Bounds.X, CMS_ListAccesClient.Bounds.Y)
                    If NewClient <> Nothing Then
                        NewClient.Trim()

                        AliasCompany(NewClient)

                        For Each u In CountryDic
                            Dim PathFileAcces As String = mPathExes + u.Value + NewClient + mExtFileAcces
                            If IO.File.Exists(PathFileAcces) Then
                                MsgBox("The client '" + NewClient + "' already exists in'" + u.Key + "'.", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - New Client")
                                Enabled_FormBox()
                                Exit Sub
                            End If
                        Next

                        Dim svDupliFileAcces As String = mPathAcces + client + mExtFileAcces
                        Dim svNewFileAcces As String = mPathAcces + NewClient + mExtFileAcces

                        If IO.File.Exists(svNewFileAcces) Then
                            IO.File.Delete(svNewFileAcces)
                        End If
                        IO.File.Copy(svDupliFileAcces, svNewFileAcces)

                        Dim CreationTime As Date = DateAndTime.Now

                        IO.File.SetCreationTime(svNewFileAcces, CreationTime)
                        IO.File.SetLastAccessTime(svNewFileAcces, CreationTime)
                        IO.File.SetLastWriteTime(svNewFileAcces, CreationTime)

                        ListAccesClient.Items.Add(NewClient)

                    End If

                Case 1 'Edit
                    If client <> Nothing Then
                        Dim WinPath As String = Environment.GetEnvironmentVariable("windir")
                        LaunchAppAndWait(WinPath + "\notepad.exe", mPathAcces + client + mExtFileAcces, ProcessWindowStyle.Normal, NOWAIT)
                    End If

                Case 2 'Delete
                    If client <> Nothing Then
                        If MsgBox("Are you sure you want to remove the client (" & client & ") ?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle)) = MsgBoxResult.Yes Then

                            If MsgBox("Are you sure you want to remove the client (" & client & ") ?", CType(MsgBoxStyle.Critical + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle)) = MsgBoxResult.Yes Then

                                AliasDic.Remove(CStr(ListAccesClient.SelectedItem))

                                Dim svFileAcces As String = mPathAcces + client + mExtFileAcces
                                If IO.File.Exists(svFileAcces) Then
                                    IO.File.Delete(svFileAcces)
                                End If

                                Dim szFile As String = CStr(client) + "." + AppendString(Crypter(CStr(client), "sha1"))
                                mConnection_DeleteFile(szFile)

                                ListAccesClient.Items.RemoveAt(ListAccesClient.SelectedIndex)

                            End If
                        End If
                    End If

                Case 3 'Add
                    If client <> Nothing Then
                        SelectedClient()
                    End If

                Case 4 'Add all
                    If client <> Nothing Then
                        SelectedAllClient()
                    End If

                Case 14 'Add all patch
                    If client <> Nothing Then
                        UnSelectedAllClient()
                        SelectedClient()
                        AddAllUpdate2Client(client)
                    End If

                Case 20
                    SelectedAllClientPige()

                Case 21
                    InvertClient()

                Case 33
                    AddGroup()

                Case 40
                    CreateSocieteAuto(client)

                Case 48

                    If client <> Nothing Then

                        Dim OldClient As String = CStr(ListAccesClient.SelectedItem)
                        Dim NewClient As String = InputBox("Rename Client:", , OldClient, CMS_ListAccesClient.Bounds.X, CMS_ListAccesClient.Bounds.Y).Trim

                        If NewClient <> Nothing Then

                            NewClient.Trim()

                            For Each u As String In AliasDic.Keys
                                If NewClient = u Then
                                    MsgBox("The client '" + NewClient + "' already exists.", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Rename Client")
                                    Enabled_FormBox()
                                    Exit Sub
                                End If
                            Next

                            For Each u As String In AliasDic.Values
                                If NewClient = u Then
                                    MsgBox("The Alias '" + NewClient + "' already exists.", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Rename Client")
                                    Enabled_FormBox()
                                    Exit Sub
                                End If
                            Next

                            AliasDic.Remove(OldClient)
                            ListAccesClient.Items.RemoveAt(ListAccesClient.SelectedIndex)

                            AliasDic.Add(NewClient, client)
                            ListAccesClient.Items.Add(NewClient)

                            Select_WriteIniString("Alias", client, NewClient, FacilityManagementIni)

                        End If

                    End If

            End Select
        End If

        Enabled_FormBox()

    End Sub

    Private Sub CMS_ListPatch_ItemClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles CMS_ListPatch.ItemClicked

        Disabled_FormBox()

        Dim str As String = e.ClickedItem.Text

        Dim int As Integer = cMSDic(str)

        Dim patch As String = CStr(ListPatch.SelectedItem)

        CMS_ListPatch.Hide()

        Select Case int

            Case 0 'New

                CreateUpdate.ShowDialog()

            Case 1 'Edit
                If patch <> Nothing Then
                    Dim WinPath As String = Environment.GetEnvironmentVariable("windir")
                    LaunchAppAndWait(WinPath + "\notepad.exe", mPathExes + ParsePath(patch, FILENAME_ONLY) + mExtFileInfo, ProcessWindowStyle.Normal, NOWAIT)
                End If

            Case 2 'Delete
                If patch <> Nothing Then
                    If MsgBox("Are you sure you want to remove the path (" & patch & ") ?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle)) = MsgBoxResult.Yes Then

                        If MsgBox("Are you sure you want to remove the path (" & patch & ") ?", CType(MsgBoxStyle.Critical + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle)) = MsgBoxResult.Yes Then

                            Dim svFilePatch As String = mPathExes + ParsePath(patch, FILENAME_ONLY) + mExtFileInfo
                            If IO.File.Exists(svFilePatch) Then
                                IO.File.Delete(svFilePatch)
                            End If

                            Dim svFileCRC As String = mPathExes + ParsePath(patch, FILENAME_ONLY) + mExtFileCRC
                            If IO.File.Exists(svFileCRC) Then
                                IO.File.Delete(svFileCRC)
                            End If

                            svFilePatch = mPathExes + patch
                            If IO.File.Exists(svFilePatch) Then
                                IO.File.Delete(svFilePatch)
                            End If

                            Dim bDeletePatch As Boolean = DicExes.Remove(patch)

                            ListPatch.Items.RemoveAt(ListPatch.SelectedIndex)

                        End If
                    End If
                End If

            Case 3 'Add
                If patch <> Nothing Then
                    SelectedPatch()
                End If

            Case 19 'refresh
                'Init_Software()
                'GetCustomerOfCountry()
                CreateListExes()
                PatchFilter()

        End Select

        Label3.Text = "Selected Updates (" + CStr(ListSelectPatch.Items.Count) + "):"
        Label4.Text = "Updates List (" + CStr(ListPatch.Items.Count) + "):"

        Enabled_FormBox()

    End Sub

    Private Sub AddAllUpdate2Client(ByRef svClient As String)

        UnSelectedAllPatch()

        Dim svPathcFileClient As String = mPathAcces + svClient + mExtFileAcces

        Dim svFileAcces() As String = IO.File.ReadAllLines(svPathcFileClient, System.Text.Encoding.Default)

        For Each j In svFileAcces

            If j = BaliseAccesEndOfFile Then Exit For

            Dim svLinePatch() As String = Split(j, ",")
            Dim svPatch As String = svLinePatch(1) 'ParsePath(svLinePatch(1), FILENAME_ONLY)

            SelectedPatch(svPatch)

        Next

    End Sub

    Private Sub Search2Customers(ByVal svFind As String, Optional ByVal UnSelected As Boolean = True)

        If UnSelected Then
            UnSelectedAllClient()
            UnSelectedAllPatch()
        End If

        For Each a In ListAccesClient.Items

            Dim svPathcFileClient As String = mPathAcces + AliasDic(CStr(a)) + mExtFileAcces

            Dim svFileAcces As String = IO.File.ReadAllText(svPathcFileClient, System.Text.Encoding.Default)

            If svFileAcces.Contains(svFind) Then
                ListSelectClient.Items.Add(a)
                'ListAccesClient.Items.Remove(a)
            End If

        Next

        For Each a In ListSelectClient.Items

            ListAccesClient.Items.Remove(a)

        Next

        Label1.Text = "Customer List (" + CStr(ListAccesClient.Items.Count) + "):"
        Label2.Text = "Selected customers (" + CStr(ListSelectClient.Items.Count) + "):"


    End Sub

    Public Function ArrayStringOf(ByVal f As ArrayList, ByVal findstring As String, Optional ByVal startindex As Integer = 0) As String

        For Each g As String In f
            Dim nvIndex As Integer = f.LastIndexOf(g)
            If g.Contains(findstring) And nvIndex > startindex Then
                Return g.Replace(findstring, Nothing)
            End If
        Next

        Return Nothing

    End Function

    Private Function CheckCRCForClient(ByVal client As String) As Boolean

        Dim svPathcFileClient As String = mPathAcces + AliasDic(client) + mExtFileAcces

        Dim svFileAcces() As String = IO.File.ReadAllLines(svPathcFileClient, System.Text.Encoding.Default)

        Dim ListOfUpdate As ArrayList = New ArrayList
        ListOfUpdate.Clear()
        ListOfUpdate.AddRange(svFileAcces)

        Dim svCRC As String = ArrayStringOf(ListOfUpdate, "%CRC%")

        If svCRC <> "" Then

            Dim index As Integer = ListOfUpdate.IndexOf("%CRC%" + svCRC)
            ListOfUpdate.RemoveAt(index)

            Dim NewCRC As String = AppendString(ArrayListToString(ListOfUpdate))

            If NewCRC = svCRC Then Return True

        End If

        MsgBox("Fichier " & Chr(34) & client & Chr(34) & " corrompu , veuillez contacter le support.", MsgBoxStyle.Critical, "Facility Management - " & MenuSoft & " - " & MenuPays)

        Return False

    End Function

    Private Function ArrayListToString(ByVal list As ArrayList) As String

        Dim listupdate As String = Nothing
        For Each u As String In list
            If listupdate = Nothing Then
                listupdate = u
            Else
                listupdate = listupdate + vbNewLine + u
            End If
        Next

        Return listupdate

    End Function

    Private Function AddCRC2String(ByVal szString As String) As String

        'AddPatchToClient

        Dim ListOfUpdate As ArrayList = New ArrayList
        ListOfUpdate.Clear()
        ListOfUpdate.AddRange(Split(szString, vbNewLine))

        Dim svCRC As String = ArrayStringOf(ListOfUpdate, "%CRC%")

        If svCRC <> "" Then

            Dim index As Integer = ListOfUpdate.IndexOf("%CRC%" + svCRC)
            ListOfUpdate.RemoveAt(index)

        End If

        Dim NewCRC As String = AppendString(ArrayListToString(ListOfUpdate))

        If NewCRC <> svCRC Then

            '' Supprime les ligne vide
            While ListOfUpdate.LastIndexOf("") > 0
                ListOfUpdate.Remove("")
            End While

            ListOfUpdate.Add("%CRC%" + NewCRC)

            szString = String.Join(vbNewLine, TryCast(ListOfUpdate.ToArray(GetType(String)), String()))

        End If

        Return szString

    End Function
    Private Sub CMS_ListSelectClient_ItemClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles CMS_ListSelectClient.ItemClicked

        Disabled_FormBox()

        If mPathAcces <> Nothing Then

            Dim str As String = e.ClickedItem.Text

            Dim int As Integer = cMSDic(str)

            Dim client As String = CStr(ListSelectClient.SelectedItem)

            If client <> Nothing Then
                client = AliasDic(client)
            End If

            CMS_ListSelectClient.Hide()

            Select Case int

                Case 1 'Edit
                    If client <> Nothing Then
                        Dim WinPath As String = Environment.GetEnvironmentVariable("windir")
                        LaunchAppAndWait(WinPath + "\notepad.exe", mPathAcces + client + mExtFileAcces, ProcessWindowStyle.Normal, NOWAIT)
                    End If

                Case 5 'clear
                    If client <> Nothing Then
                        UnSelectedClient()
                    End If

                Case 6 'Clear all
                    UnSelectedAllClient()

                Case 9
                    AddPatchToClient(client)

                Case 10
                    Send2FTP(client)

                Case 14 'Add all patch
                    AddAllUpdate2Client(client)

                Case 21
                    InvertClient()

                Case 49 'Skip
                    SkipPatchToClient(client)

                Case 50 'Not Skip
                    NotSkipPatchToClient()

                Case 52 'Adm
                    AdmPatchToClient(client)

                Case 53 'Not Adm
                    NotAdmPatchToClient()

            End Select
        End If

        Enabled_FormBox()

    End Sub

    Private Sub CMS_ListSelectPatch_ItemClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles CMS_ListSelectPatch.ItemClicked

        Disabled_FormBox()

        Dim str As String = e.ClickedItem.Text

        Dim int As Integer = cMSDic(str)

        Dim Patch As String = CStr(ListSelectPatch.SelectedItem)

        CMS_ListSelectPatch.Hide()

        Select Case int

            Case 1 'Edit
                If Patch <> Nothing Then
                    Dim WinPath As String = Environment.GetEnvironmentVariable("windir")
                    LaunchAppAndWait(WinPath + "\notepad.exe", mPathExes + ParsePath(Patch, FILENAME_ONLY) + mExtFileInfo, ProcessWindowStyle.Normal, NOWAIT)
                End If

            Case 5 'clear
                If Patch <> Nothing Then
                    UnSelectedPatch()
                End If

            Case 6 'Clear all
                UnSelectedAllPatch()

            Case 9
                AddPatchToClient(, Patch)

            Case 10
                Send2FTP(, Patch)

            Case 49 'SKIP
                SkipPatchToClient(, Patch)

            Case 50 'Not SKIP
                NotSkipPatchToClient(, Patch)

            Case 52 'ADM
                AdmPatchToClient(, Patch)

            Case 53 'Not ADM
                NotAdmPatchToClient(, Patch)

        End Select

        Enabled_FormBox()

    End Sub

    Private Sub AddPatchToClient(Optional ByVal svOnlySelectedClient As String = Nothing, Optional ByVal svOnlySeclectedPatch As String = Nothing, Optional ByVal MsgFinishVisible As Boolean = True, Optional ByVal PatchLessOne As String = Nothing)

        Disabled_FormBox()

        If ListSelectClient.Items.Count = 0 Or ListSelectPatch.Items.Count = 0 Then
            If MsgFinishVisible Then
                MsgBox("No selection !", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Add the path for customers")
            End If
            Enabled_FormBox()
            Exit Sub
        End If

        If MsgFinishVisible Then
            If MsgBox("Are you sure you want to add the patch ?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle)) = MsgBoxResult.No Then
                MsgBox("No selection !", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Add the patch for customers")
                Enabled_FormBox()
                Exit Sub
            End If
        End If

        '******************* Olive ********************
        Dim ListPAT = New System.Windows.Forms.ListBox
        If svOnlySeclectedPatch = Nothing Then
            ListPAT = ListSelectPatch
        Else
            ListPAT.Items.Add(svOnlySeclectedPatch)
        End If
        '******************* Olive ********************

        Dim sAddLine(ListPAT.Items.Count - 1) As PATCH_INFO

        Dim Index As Integer = 0

        For Each j In ListPAT.Items

            Dim svPathcFileTxt As String = mPathExes + ParsePath(CStr(j), FILENAME_ONLY) + mExtFileInfo
            Dim svPathcFileExe As String = mPathExes + CStr(j)

            sAddLine(Index).patch = CStr(j)
            sAddLine(Index).size = CStr(FileLen(svPathcFileExe))
            sAddLine(Index).crc = Append(svPathcFileExe)

            sAddLine(Index).info = Replace(Join(IO.File.ReadAllLines(svPathcFileTxt, System.Text.Encoding.Default), "\r\n"), ",", ";")

            Index += 1

        Next

        'https://mp.kantarmedia.fr/fret2/medial/gs.txt

        Dim Verif As PATCH_INFO = New PATCH_INFO
        If PatchLessOne <> Nothing Then

            Dim svPathcFileTxt As String = mPathExes + ParsePath(CStr(PatchLessOne), FILENAME_ONLY) + mExtFileInfo
            Dim svPathcFileExe As String = mPathExes + CStr(PatchLessOne)

            If IO.File.Exists(svPathcFileExe) Then

                Verif.patch = CStr(PatchLessOne)
                Verif.size = CStr(FileLen(svPathcFileExe))
                Verif.crc = Append(svPathcFileExe)
                Verif.info = Replace(Join(IO.File.ReadAllLines(svPathcFileTxt, System.Text.Encoding.Default), "\r\n"), ",", ";")

            End If


        End If

        '******************* Olive ********************
        Dim ListCLI = New System.Windows.Forms.ListBox
        If svOnlySelectedClient = Nothing Then
            ListCLI = ListSelectClient
        Else
            ListCLI.Items.Add(svOnlySelectedClient)
        End If
        '******************* Olive ********************

        Dim RemoveClient As ArrayList = New ArrayList

        For Each g In ListCLI.Items

            If CheckCRCForClient(g.ToString) Then

                Dim AddPatch As Boolean = False

                Dim svPathcFileClient As String = mPathAcces + AliasDic(CStr(g)) + mExtFileAcces

                Dim svPathcFileClientTmp As String = mPathAcces + AliasDic(CStr(g)) + mExtFileAcces + mExtFileTmp

                Dim svFileAcces() As String = IO.File.ReadAllLines(svPathcFileClient, System.Text.Encoding.Default)


                '*********************************************************************
                '     GROS BUG D'INDEX (ItemsCount vs nvCount) si debut Maj > 1
                '*********************************************************************

                Dim ItemsCount As Integer = Array.FindIndex(svFileAcces, AddressOf FindByDescription)


                '********************************************************************
                '                   Sauvegarde des options
                '********************************************************************
                Dim svOption As String = Nothing
                If ItemsCount + 1 < svFileAcces.Count Then
                    If svFileAcces(ItemsCount + 1).Trim <> Nothing Then
                        For nvOption As Integer = ItemsCount + 1 To svFileAcces.Count - 1
                            svOption = svOption + vbNewLine + svFileAcces(nvOption)
                        Next
                        ReDim Preserve svFileAcces(ItemsCount) 'Ajout le 19 Janvier 2018 : Suppression des lignes d'options
                    End If
                End If


                Dim nvCount As Integer = 0

                If ItemsCount > 0 Then
                    Dim LineCount() As String = Split(svFileAcces(ItemsCount - 1), ",")
                    nvCount = CInt(Val(LineCount(0)))
                End If

                ReDim Preserve svFileAcces(ItemsCount + Index)

                For Each v In sAddLine

                    If PatchLessOne <> Nothing Then

                        If Verif.FindString(svFileAcces) = Nothing Then
                            'MsgBox("this patch " & Chr(34) & Verif.patch & Chr(34) & " is not present for the client: " & CStr(g), MsgBoxStyle.Exclamation)
                            If Verif.patch <> Nothing Then
                                If MsgBox("this patch " & Chr(34) & Verif.patch & Chr(34) & " is not present for the client: " & CStr(g) + vbNewLine + vbNewLine + "Are you sure you want to add the patch (" & Chr(34) & v.patch & Chr(34) & ") ?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton1, MsgBoxStyle), "Patch Pige") = MsgBoxResult.No Then
                                    RemoveClient.Add(CStr(g))
                                    AddPatch = False
                                    Exit For
                                End If
                            End If
                        End If
                    End If

                    System.Windows.Forms.Application.DoEvents()

                    Dim svMaj As String = v.FindString(svFileAcces)

                    If svMaj <> Nothing Then
                        MsgBox("this patch " & Chr(34) & v.patch & Chr(34) & " (Number: " & svMaj & ") is already present for the client: " & CStr(g), MsgBoxStyle.Exclamation)
                        RemoveClient.Add(CStr(g))
                    Else
                        v.count = CStr(nvCount + 1)

                        svFileAcces(ItemsCount) = v.ToString()

                        ItemsCount += 1

                        nvCount += 1

                        If Not AddPatch Then
                            AddPatch = True

                            'Entre la version 1.1.9.3 et la 1.1.9.4
                            'Exit For !!!

                        End If

                    End If

                Next

                If AddPatch Then

                    svFileAcces(ItemsCount) = BaliseAccesEndOfFile

                    ReDim Preserve svFileAcces(ItemsCount)

                    If IO.File.Exists(svPathcFileClientTmp) Then
                        IO.File.Delete(svPathcFileClientTmp)
                    End If
                    IO.File.Move(svPathcFileClient, svPathcFileClientTmp)

                    IO.File.WriteAllText(svPathcFileClient, AddCRC2String(Join(svFileAcces, vbNewLine) + svOption), System.Text.Encoding.Default)

                    If IO.File.Exists(svPathcFileClient) And FileLen(svPathcFileClient) <> 0 Then
                        IO.File.Delete(svPathcFileClientTmp)
                    End If

                End If
            End If
        Next

        If RemoveClient.Count > 0 Then
            For Each g As String In RemoveClient
                ListAccesClient.Items.Add(g)
                ListSelectClient.Items.Remove(g)
            Next

            If ListSelectClient.Items.Count = 0 Then
                UnSelectedAllPatch()
            End If
        End If


        If MsgFinishVisible Then
            MsgBox("Completed.", CType(MsgBoxStyle.Information + MsgBoxStyle.OkOnly + MsgBoxStyle.SystemModal, MsgBoxStyle))
        End If

        ToolStripStatusLabelFM.Text = Nothing
        ToolStripProgressBar1.Value = 0

        Enabled_FormBox()

    End Sub

    Private Sub AddSkipToClient(Optional ByVal svOnlySelectedClient As String = Nothing, Optional ByVal svOnlySeclectedPatch As String = Nothing, Optional ByVal MsgFinishVisible As Boolean = True, Optional ByVal PatchLessOne As String = Nothing)

        Disabled_FormBox()

        If ListSelectClient.Items.Count = 0 Or ListSelectPatch.Items.Count = 0 Then
            MsgBox("No selection !", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Add the path for customers")
            Enabled_FormBox()
            Exit Sub
        End If

        If MsgFinishVisible Then
            If MsgBox("Are you sure you want to add the patch ?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle)) = MsgBoxResult.No Then
                MsgBox("No selection !", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Add the patch for customers")
                Enabled_FormBox()
                Exit Sub
            End If
        End If

        '******************* Olive ********************
        Dim ListPAT = New System.Windows.Forms.ListBox
        If svOnlySeclectedPatch = Nothing Then
            ListPAT = ListSelectPatch
        Else
            ListPAT.Items.Add(svOnlySeclectedPatch)
        End If
        '******************* Olive ********************

        Dim sAddLine(ListPAT.Items.Count - 1) As PATCH_INFO

        Dim Index As Integer = 0

        For Each j In ListPAT.Items

            Dim svPathcFileTxt As String = mPathExes + ParsePath(CStr(j), FILENAME_ONLY) + mExtFileInfo
            Dim svPathcFileExe As String = mPathExes + CStr(j)

            sAddLine(Index).patch = CStr(j)
            sAddLine(Index).size = CStr(FileLen(svPathcFileExe))
            sAddLine(Index).crc = Append(svPathcFileExe)

            sAddLine(Index).info = Replace(Join(IO.File.ReadAllLines(svPathcFileTxt, System.Text.Encoding.Default), "\r\n"), ",", ";")

            Index += 1

        Next

        'https://mp.kantarmedia.fr/fret2/medial/gs.txt

        Dim Verif As PATCH_INFO = New PATCH_INFO
        If PatchLessOne <> Nothing Then

            Dim svPathcFileTxt As String = mPathExes + ParsePath(CStr(PatchLessOne), FILENAME_ONLY) + mExtFileInfo
            Dim svPathcFileExe As String = mPathExes + CStr(PatchLessOne)

            If IO.File.Exists(svPathcFileExe) Then

                Verif.patch = CStr(PatchLessOne)
                Verif.size = CStr(FileLen(svPathcFileExe))
                Verif.crc = Append(svPathcFileExe)
                Verif.info = Replace(Join(IO.File.ReadAllLines(svPathcFileTxt, System.Text.Encoding.Default), "\r\n"), ",", ";")

            End If


        End If

        '******************* Olive ********************
        Dim ListCLI = New System.Windows.Forms.ListBox
        If svOnlySelectedClient = Nothing Then
            ListCLI = ListSelectClient
        Else
            ListCLI.Items.Add(svOnlySelectedClient)
        End If
        '******************* Olive ********************

        Dim RemoveClient As ArrayList = New ArrayList

        For Each g In ListCLI.Items

            If CheckCRCForClient(g.ToString) Then

                Dim AddPatch As Boolean = False

                Dim svPathcFileClient As String = mPathAcces + AliasDic(CStr(g)) + mExtFileAcces

                Dim svPathcFileClientTmp As String = mPathAcces + AliasDic(CStr(g)) + mExtFileAcces + mExtFileTmp

                Dim svFileAcces() As String = IO.File.ReadAllLines(svPathcFileClient, System.Text.Encoding.Default)


                '*********************************************************************
                '     GROS BUG D'INDEX (ItemsCount vs nvCount) si debut Maj > 1
                '*********************************************************************

                Dim ItemsCount As Integer = Array.FindIndex(svFileAcces, AddressOf FindByDescription)


                '********************************************************************
                '                   Sauvegarde des options
                '********************************************************************
                Dim svOption As String = Nothing
                If ItemsCount + 1 < svFileAcces.Count Then
                    If svFileAcces(ItemsCount + 1).Trim <> Nothing Then
                        For nvOption As Integer = ItemsCount + 1 To svFileAcces.Count - 1
                            svOption = svOption + vbNewLine + svFileAcces(nvOption)
                        Next
                        ReDim Preserve svFileAcces(ItemsCount) 'Ajout le 19 Janvier 2018 : Suppression des lignes d'options
                    End If
                End If


                Dim nvCount As Integer = 0

                If ItemsCount > 0 Then
                    Dim LineCount() As String = Split(svFileAcces(ItemsCount - 1), ",")
                    nvCount = CInt(Val(LineCount(0)))
                End If

                ReDim Preserve svFileAcces(ItemsCount + Index)

                For Each v In sAddLine

                    If PatchLessOne <> Nothing Then

                        If Verif.FindString(svFileAcces) = Nothing Then
                            'MsgBox("this patch " & Chr(34) & Verif.patch & Chr(34) & " is not present for the client: " & CStr(g), MsgBoxStyle.Exclamation)
                            If Verif.patch <> Nothing Then
                                If MsgBox("this patch " & Chr(34) & Verif.patch & Chr(34) & " is not present for the client: " & CStr(g) + vbNewLine + vbNewLine + "Are you sure you want to add the patch (" & Chr(34) & v.patch & Chr(34) & ") ?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton1, MsgBoxStyle), "Patch Pige") = MsgBoxResult.No Then
                                    RemoveClient.Add(CStr(g))
                                    AddPatch = False
                                    Exit For
                                End If
                            End If
                        End If
                    End If

                    System.Windows.Forms.Application.DoEvents()

                    Dim svMaj As String = v.FindString(svFileAcces)

                    If svMaj <> Nothing Then
                        MsgBox("this patch " & Chr(34) & v.patch & Chr(34) & " (Number: " & svMaj & ") is already present for the client: " & CStr(g), MsgBoxStyle.Exclamation)
                        RemoveClient.Add(CStr(g))
                    Else
                        v.count = CStr(nvCount + 1)

                        svFileAcces(ItemsCount) = v.ToString()

                        ItemsCount += 1

                        nvCount += 1

                        If Not AddPatch Then
                            AddPatch = True

                            'Entre la version 1.1.9.3 et la 1.1.9.4
                            'Exit For !!!

                        End If

                    End If


                Next

                If AddPatch Then

                    svFileAcces(ItemsCount) = BaliseAccesEndOfFile

                    ReDim Preserve svFileAcces(ItemsCount)

                    If IO.File.Exists(svPathcFileClientTmp) Then
                        IO.File.Delete(svPathcFileClientTmp)
                    End If
                    IO.File.Move(svPathcFileClient, svPathcFileClientTmp)

                    IO.File.WriteAllText(svPathcFileClient, AddCRC2String(Join(svFileAcces, vbNewLine) + svOption), System.Text.Encoding.Default)

                    If IO.File.Exists(svPathcFileClient) And FileLen(svPathcFileClient) <> 0 Then
                        IO.File.Delete(svPathcFileClientTmp)
                    End If

                End If

            End If

        Next

        If RemoveClient.Count > 0 Then
            For Each g As String In RemoveClient
                ListAccesClient.Items.Add(g)
                ListSelectClient.Items.Remove(g)
            Next

            If ListSelectClient.Items.Count = 0 Then
                UnSelectedAllPatch()
            End If
        End If


        If MsgFinishVisible Then
            MsgBox("Completed.", CType(MsgBoxStyle.Information + MsgBoxStyle.OkOnly + MsgBoxStyle.SystemModal, MsgBoxStyle))
        End If

        ToolStripStatusLabelFM.Text = Nothing
        ToolStripProgressBar1.Value = 0

        Enabled_FormBox()

    End Sub

    Private Sub Add2Customer(ByVal svAdd As String, Optional ByVal svOnlySelectedClient As String = Nothing, Optional ByVal MsgFinishVisible As Boolean = True)

        Disabled_FormBox()

        If ListSelectClient.Items.Count = 0 Then
            If MsgFinishVisible Then
                MsgBox("No selection !", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Add " + svAdd)
            End If
            Enabled_FormBox()
            Exit Sub
        End If

        '******************* Olive ********************
        Dim ListCLI = New System.Windows.Forms.ListBox
        If svOnlySelectedClient = Nothing Then
            ListCLI = ListSelectClient
        Else
            ListCLI.Items.Add(svOnlySelectedClient)
        End If
        '******************* Olive ********************

        For Each g In ListCLI.Items

            If CheckCRCForClient(g.ToString) Then

                Dim svPathcFileClient As String = mPathAcces + AliasDic(CStr(g)) + mExtFileAcces

                Dim ListOfUpdate As ArrayList = New ArrayList

                Dim FileAcces As String = IO.File.ReadAllText(svPathcFileClient, System.Text.Encoding.Default)

                ListOfUpdate.Clear()
                ListOfUpdate.AddRange(Split(FileAcces, vbNewLine))

                Dim IndexListEnd As Integer = ListOfUpdate.IndexOf(BaliseAccesEndOfFile)

                Dim RequestInfo As Boolean = (ListOfUpdate.IndexOf(svAdd) > IndexListEnd)

                If Not RequestInfo Then
                    ListOfUpdate.Add(svAdd)

                    FileAcces = Join(ListOfUpdate.ToArray, vbNewLine)

                    IO.File.WriteAllText(svPathcFileClient, AddCRC2String(FileAcces), System.Text.Encoding.Default)

                End If
            End If

        Next


        If MsgFinishVisible Then
            MsgBox("Completed.", CType(MsgBoxStyle.Information + MsgBoxStyle.OkOnly + MsgBoxStyle.SystemModal, MsgBoxStyle))
        End If

        Enabled_FormBox()

    End Sub

    Private Sub SkipPatchToClient(Optional ByVal svOnlySelectedClient As String = Nothing, Optional ByVal svOnlySeclectedPatch As String = Nothing)

        Disabled_FormBox()

        If ListSelectClient.Items.Count = 0 Or ListSelectPatch.Items.Count = 0 Then
            MsgBox("No selection !", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Skip these patch for customers")
            Enabled_FormBox()
            Exit Sub
        End If

        If MsgBox("Are you sure you want to skip these patch ?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle)) = MsgBoxResult.No Then
            Enabled_FormBox()
            Exit Sub
        End If

        '******************* Olive ********************
        Dim ListPAT = New System.Windows.Forms.ListBox
        If svOnlySeclectedPatch = Nothing Then
            ListPAT = ListSelectPatch
        Else
            ListPAT.Items.Add(svOnlySeclectedPatch)
        End If

        Dim ListCLI = New System.Windows.Forms.ListBox
        If svOnlySelectedClient = Nothing Then
            ListCLI = ListSelectClient
        Else
            ListCLI.Items.Add(svOnlySelectedClient)
        End If
        '******************* Olive ********************

        For Each g In ListCLI.Items

            If CheckCRCForClient(g.ToString) Then

                Dim WriteFile As Boolean = False

                Dim svPathcFileClient As String = mPathAcces + AliasDic(CStr(g)) + mExtFileAcces

                Dim svFileAcces() As String = IO.File.ReadAllLines(svPathcFileClient, System.Text.Encoding.Default)

                For Each v In ListPAT.Items

                    For Each j As String In svFileAcces

                        If j = BaliseAccesEndOfFile Then
                            Exit For
                        End If

                        Dim svLinePatch() As String = Split(j, ",")

                        If svLinePatch(1) = v.ToString And Not svLinePatch(4).Contains(Skip) Then
                            If Not WriteFile Then WriteFile = True
                            svLinePatch(4) = svLinePatch(4) + Skip
                            svFileAcces(Array.IndexOf(svFileAcces, j)) = Join(svLinePatch, ",")
                        End If

                    Next

                Next

                If WriteFile Then
                    IO.File.WriteAllText(svPathcFileClient, AddCRC2String(Join(svFileAcces, vbNewLine)), System.Text.Encoding.Default)
                End If

            End If

        Next

        MsgBox("Completed.", CType(MsgBoxStyle.Information + MsgBoxStyle.OkOnly + MsgBoxStyle.SystemModal, MsgBoxStyle))

        Enabled_FormBox()

    End Sub

    Private Sub NotSkipPatchToClient(Optional ByVal svOnlySelectedClient As String = Nothing, Optional ByVal svOnlySeclectedPatch As String = Nothing)

        Disabled_FormBox()

        If ListSelectClient.Items.Count = 0 Or ListSelectPatch.Items.Count = 0 Then
            MsgBox("No selection !", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Skip these patch for customers")
            Enabled_FormBox()
            Exit Sub
        End If

        If MsgBox("Are you sure you want to not skip these patch ?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle)) = MsgBoxResult.No Then
            Enabled_FormBox()
            Exit Sub
        End If

        '******************* Olive ********************
        Dim ListPAT = New System.Windows.Forms.ListBox
        If svOnlySeclectedPatch = Nothing Then
            ListPAT = ListSelectPatch
        Else
            ListPAT.Items.Add(svOnlySeclectedPatch)
        End If

        Dim ListCLI = New System.Windows.Forms.ListBox
        If svOnlySelectedClient = Nothing Then
            ListCLI = ListSelectClient
        Else
            ListCLI.Items.Add(svOnlySelectedClient)
        End If
        '******************* Olive ********************

        For Each g In ListCLI.Items

            If CheckCRCForClient(g.ToString) Then

                Dim WriteFile As Boolean = False

                Dim svPathcFileClient As String = mPathAcces + AliasDic(CStr(g)) + mExtFileAcces

                Dim svFileAcces() As String = IO.File.ReadAllLines(svPathcFileClient, System.Text.Encoding.Default)

                For Each v In ListPAT.Items

                    For Each j As String In svFileAcces

                        If j = BaliseAccesEndOfFile Then
                            Exit For
                        End If

                        Dim svLinePatch() As String = Split(j, ",")

                        If svLinePatch(1) = v.ToString And svLinePatch(4).Contains(Skip) Then
                            If Not WriteFile Then WriteFile = True
                            svLinePatch(4) = svLinePatch(4).Replace(Skip, "")
                            svFileAcces(Array.IndexOf(svFileAcces, j)) = Join(svLinePatch, ",")
                        End If

                    Next

                Next

                If WriteFile Then
                    IO.File.WriteAllText(svPathcFileClient, AddCRC2String(Join(svFileAcces, vbNewLine)), System.Text.Encoding.Default)
                End If

            End If

        Next

        MsgBox("Completed.", CType(MsgBoxStyle.Information + MsgBoxStyle.OkOnly + MsgBoxStyle.SystemModal, MsgBoxStyle))

        Enabled_FormBox()

    End Sub

    Private Sub AdmPatchToClient(Optional ByVal svOnlySelectedClient As String = Nothing, Optional ByVal svOnlySeclectedPatch As String = Nothing)

        Disabled_FormBox()

        If ListSelectClient.Items.Count = 0 Or ListSelectPatch.Items.Count = 0 Then
            MsgBox("No selection !", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Admin patch for customers")
            Enabled_FormBox()
            Exit Sub
        End If

        If MsgBox("Are you sure you want to admin these patch ?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle)) = MsgBoxResult.No Then
            Enabled_FormBox()
            Exit Sub
        End If

        '******************* Olive ********************
        Dim ListPAT = New System.Windows.Forms.ListBox
        If svOnlySeclectedPatch = Nothing Then
            ListPAT = ListSelectPatch
        Else
            ListPAT.Items.Add(svOnlySeclectedPatch)
        End If

        Dim ListCLI = New System.Windows.Forms.ListBox
        If svOnlySelectedClient = Nothing Then
            ListCLI = ListSelectClient
        Else
            ListCLI.Items.Add(svOnlySelectedClient)
        End If
        '******************* Olive ********************

        For Each g In ListCLI.Items

            If CheckCRCForClient(g.ToString) Then

                Dim WriteFile As Boolean = False

                Dim svPathcFileClient As String = mPathAcces + AliasDic(CStr(g)) + mExtFileAcces

                Dim svFileAcces() As String = IO.File.ReadAllLines(svPathcFileClient, System.Text.Encoding.Default)

                For Each v In ListPAT.Items

                    For Each j As String In svFileAcces

                        If j = BaliseAccesEndOfFile Then
                            Exit For
                        End If

                        Dim svLinePatch() As String = Split(j, ",")

                        If svLinePatch(1) = v.ToString And Not svLinePatch(4).Contains(Adm) Then
                            If Not WriteFile Then WriteFile = True
                            svLinePatch(4) = svLinePatch(4) + Adm
                            svFileAcces(Array.IndexOf(svFileAcces, j)) = Join(svLinePatch, ",")
                        End If

                    Next

                Next

                If WriteFile Then
                    IO.File.WriteAllText(svPathcFileClient, AddCRC2String(Join(svFileAcces, vbNewLine)), System.Text.Encoding.Default)
                End If

            End If

        Next

        MsgBox("Completed.", CType(MsgBoxStyle.Information + MsgBoxStyle.OkOnly + MsgBoxStyle.SystemModal, MsgBoxStyle))

        Enabled_FormBox()

    End Sub

    Private Sub NotAdmPatchToClient(Optional ByVal svOnlySelectedClient As String = Nothing, Optional ByVal svOnlySeclectedPatch As String = Nothing)

        Disabled_FormBox()

        If ListSelectClient.Items.Count = 0 Or ListSelectPatch.Items.Count = 0 Then
            MsgBox("No selection !", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Admin patch for customers")
            Enabled_FormBox()
            Exit Sub
        End If

        If MsgBox("Are you sure you want to not admin these patch ?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle)) = MsgBoxResult.No Then
            Enabled_FormBox()
            Exit Sub
        End If

        '******************* Olive ********************
        Dim ListPAT = New System.Windows.Forms.ListBox
        If svOnlySeclectedPatch = Nothing Then
            ListPAT = ListSelectPatch
        Else
            ListPAT.Items.Add(svOnlySeclectedPatch)
        End If

        Dim ListCLI = New System.Windows.Forms.ListBox
        If svOnlySelectedClient = Nothing Then
            ListCLI = ListSelectClient
        Else
            ListCLI.Items.Add(svOnlySelectedClient)
        End If
        '******************* Olive ********************

        For Each g In ListCLI.Items

            If CheckCRCForClient(g.ToString) Then

                Dim WriteFile As Boolean = False

                Dim svPathcFileClient As String = mPathAcces + AliasDic(CStr(g)) + mExtFileAcces

                Dim svFileAcces() As String = IO.File.ReadAllLines(svPathcFileClient, System.Text.Encoding.Default)

                For Each v In ListPAT.Items

                    For Each j As String In svFileAcces

                        If j = BaliseAccesEndOfFile Then
                            Exit For
                        End If

                        Dim svLinePatch() As String = Split(j, ",")

                        If svLinePatch(1) = v.ToString And svLinePatch(4).Contains(Adm) Then
                            If Not WriteFile Then WriteFile = True
                            svLinePatch(4) = svLinePatch(4).Replace(Adm, "")
                            svFileAcces(Array.IndexOf(svFileAcces, j)) = Join(svLinePatch, ",")
                        End If

                    Next

                Next

                If WriteFile Then
                    IO.File.WriteAllText(svPathcFileClient, AddCRC2String(Join(svFileAcces, vbNewLine)), System.Text.Encoding.Default)
                End If

            End If

        Next

        MsgBox("Completed.", CType(MsgBoxStyle.Information + MsgBoxStyle.OkOnly + MsgBoxStyle.SystemModal, MsgBoxStyle))

        Enabled_FormBox()

    End Sub

    Function FindByDescription(ByVal item As String) As Boolean

        If item = BaliseAccesEndOfFile Then
            Return True
        Else
            Return False
        End If

    End Function

    Private Sub ToolTipInfo(ByVal list As ListBox, ByVal e As System.Windows.Forms.MouseEventArgs, ByVal mPath As String, ByVal mExt As String)

        Dim OverIndex As Integer = list.IndexFromPoint(e.X, e.Y)

        If list.Items.Count > OverIndex And list.SelectedIndex <> OverIndex Then
            list.SelectedIndex = OverIndex
            Dim pathfileinfo As String = mPath + ParsePath(CStr(list.SelectedItem), FILENAME_ONLY) + mExt
            If IO.File.Exists(pathfileinfo) Then

                Dim fileinfo As String = IO.File.ReadAllText(pathfileinfo, System.Text.Encoding.Default)

                'Dim fileinfo As String = IO.File.ReadAllText(pathfileinfo, System.Text.Encoding.UTF7)

                'If ToolTip1.GetToolTip(list) <> fileinfo Then
                ToolTip1.SetToolTip(list, fileinfo)
                System.Windows.Forms.Application.DoEvents()
                'End If
            Else
                ToolTip1.SetToolTip(list, Nothing)
            End If

        End If

        If OverIndex = -1 Then
            ToolTip1.SetToolTip(list, Nothing)
        End If

    End Sub

    Private Sub ToolTipInfoAlias(ByVal list As ListBox, ByVal e As System.Windows.Forms.MouseEventArgs)

        Dim OverIndex As Integer = list.IndexFromPoint(e.X, e.Y)

        If list.Items.Count > OverIndex Then 'And list.SelectedIndex <> OverIndex Then
            list.SelectedIndex = OverIndex
            If CStr(list.SelectedItem) <> "" Then
                If AliasDic(CStr(list.SelectedItem)) <> CStr(list.SelectedItem) Then
                    ToolTip1.SetToolTip(list, AliasDic(CStr(list.SelectedItem)))
                    System.Windows.Forms.Application.DoEvents()
                    'End If
                Else
                    ToolTip1.SetToolTip(list, Nothing)
                End If
            End If
        End If

        If OverIndex = -1 Then
            ToolTip1.SetToolTip(list, Nothing)
        End If


    End Sub

    Private Sub ListAccesClient_MouseDown(sender As Object, e As System.Windows.Forms.MouseEventArgs) Handles ListAccesClient.MouseDown
        If e.Button = MouseButtons.Right Then
            ListAccesClient_mDown = True
        End If
    End Sub

    Private Sub ListAccesClient_MouseLeave(sender As Object, e As System.EventArgs) Handles ListAccesClient.MouseLeave
        If ListAccesClient_mDown = False Then
            ListAccesClient.SelectedIndex = -1
        Else
            ListAccesClient_mDown = False
        End If

    End Sub

    Private Sub ListAccesClient_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListAccesClient.MouseMove

        ToolTipInfoAlias(ListAccesClient, e)

        'Dim OverIndex As Integer = ListAccesClient.IndexFromPoint(e.X, e.Y)

        'If ListAccesClient.Items.Count > OverIndex Then
        '    ListAccesClient.SelectedIndex = OverIndex
        'End If

    End Sub

    Private Sub ListSelectClient_MouseDown(sender As Object, e As System.Windows.Forms.MouseEventArgs) Handles ListSelectClient.MouseDown
        If e.Button = MouseButtons.Right Then
            ListSelectClient_mDown = True
        End If
    End Sub

    Private Sub ListSelectClient_MouseLeave(sender As Object, e As System.EventArgs) Handles ListSelectClient.MouseLeave
        If ListSelectClient_mDown = False Then
            ListSelectClient.SelectedIndex = -1
        Else
            ListSelectClient_mDown = False
        End If
    End Sub

    Private Sub ListSelectClient_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListSelectClient.MouseMove

        ToolTipInfoAlias(ListSelectClient, e)

        'Dim OverIndex As Integer = ListSelectClient.IndexFromPoint(e.X, e.Y)

        'If ListSelectClient.Items.Count > OverIndex Then
        '    ListSelectClient.SelectedIndex = OverIndex
        'End If

    End Sub

    Private Sub CMS_ListAccesClient_Opening(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles CMS_ListAccesClient.Opening

        CMS_ListAccesClient.Items.Clear()

        'Alias
        Dim client As String = CStr(ListAccesClient.SelectedItem)
        If client <> Nothing Then
            client = AliasDic(client)
            If StrComp(CStr(ListAccesClient.SelectedItem), client, CompareMethod.Text) <> 0 Then

                AliasCustomer = New System.Windows.Forms.ToolStripMenuItem With {
                    .Text = cMSDic.Keys(34),
                    .Font = New System.Drawing.Font("Tahoma", 16.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte)),
                    .ImageScaling = ToolStripItemImageScaling.None,
                    .AutoSize = False,
                    .Size = New System.Drawing.Size(160, 40),
                    .Image = Facility_Management.My.Resources.users_into
                }

                AliasCustomer.DropDownItems.Add(client.Replace("&", "&&"))
                AliasCustomer.DropDownItems.Item(0).ImageScaling = ToolStripItemImageScaling.None
                AliasCustomer.DropDownItems.Item(0).Image = Facility_Management.My.Resources.personne

                CMS_ListAccesClient.Items.AddRange(New System.Windows.Forms.ToolStripItem() {AliasCustomer})

            End If
        End If

        CMS_ListAccesClient.Items.Add(cMSDic.Keys(0), Facility_Management.My.Resources.Create)
        CMS_ListAccesClient.Items.Item(CMS_ListAccesClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        If client <> Nothing Then

            CMS_ListAccesClient.Items.Add(cMSDic.Keys(8), Facility_Management.My.Resources.Dupliquer)
            CMS_ListAccesClient.Items.Item(CMS_ListAccesClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            CMS_ListAccesClient.Items.Add(cMSDic.Keys(3), Facility_Management.My.Resources.Add)
            CMS_ListAccesClient.Items.Item(CMS_ListAccesClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            CMS_ListAccesClient.Items.Add(cMSDic.Keys(4), Facility_Management.My.Resources.Ajouter)
            CMS_ListAccesClient.Items.Item(CMS_ListAccesClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

            'Add Groups
            Dim svGroup As String = Select_GetIniString(MenuSoft, "Group1".Trim, FacilityManagementIni).Trim

            If svGroup <> Nothing Then

                Dim nvCountGroup As Integer = 1

                Group = New System.Windows.Forms.ToolStripMenuItem With {
                    .Text = cMSDic.Keys(33),
                    .Image = Facility_Management.My.Resources.Add_all_icon
                }

                Do

                    Dim Clients() As String = Split(svGroup, ",")


                    Customer = New System.Windows.Forms.ToolStripMenuItem
                    For nvCustomer As Integer = 1 To Clients.Length - 1
                        Customer.DropDownItems.Add(Clients(nvCustomer).Replace("&", "&&"))
                        Customer.DropDownItems.Item(nvCustomer - 1).ImageScaling = ToolStripItemImageScaling.None
                        Customer.DropDownItems.Item(nvCustomer - 1).Image = Facility_Management.My.Resources.personne
                    Next

                    Group.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Customer})

                    Group.DropDownItems.Item(nvCountGroup - 1).Text = Clients(0)
                    Group.DropDownItems.Item(nvCountGroup - 1).ImageScaling = ToolStripItemImageScaling.None
                    Group.DropDownItems.Item(nvCountGroup - 1).Image = Facility_Management.My.Resources.utilisateurs

                    nvCountGroup += 1

                    svGroup = Select_GetIniString(MenuSoft, "Group" + CStr(nvCountGroup).Trim, FacilityManagementIni).Trim
                    If svGroup = Nothing Then Exit Do

                Loop

                CMS_ListAccesClient.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.Group})
                CMS_ListAccesClient.Items.Item(CMS_ListAccesClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

            End If

            CMS_ListAccesClient.Items.Add(cMSDic.Keys(1), Facility_Management.My.Resources.Edit)
            CMS_ListAccesClient.Items.Item(CMS_ListAccesClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            CMS_ListAccesClient.Items.Add(cMSDic.Keys(2), Facility_Management.My.Resources.corbeille)
            CMS_ListAccesClient.Items.Item(CMS_ListAccesClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            CMS_ListAccesClient.Items.Add(cMSDic.Keys(14), Facility_Management.My.Resources.Add_package)
            CMS_ListAccesClient.Items.Item(CMS_ListAccesClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        End If

        CMS_ListAccesClient.Items.Add(cMSDic.Keys(40), Facility_Management.My.Resources.Wizard_Wand)
        CMS_ListAccesClient.Items.Item(CMS_ListAccesClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None


        'Pige
        If Val(Select_GetIniString(MenuSoft, "ToolsMenuPige", FacilityManagementIni).Trim) = 1 Then
            CMS_ListAccesClient.Items.Add(cMSDic.Keys(20), Facility_Management.My.Resources.Wizard_Wand)
            CMS_ListAccesClient.Items.Item(CMS_ListAccesClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        End If

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub CMS_ListSelectClient_Opening(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles CMS_ListSelectClient.Opening

        CMS_ListSelectClient.Items.Clear()

        Dim client As String = CStr(ListSelectClient.SelectedItem)

        If client <> Nothing Then
            client = AliasDic(client)
        End If

        If client <> Nothing Then

            'Olive
            CMS_ListSelectClient.Items.Add(cMSDic.Keys(10), Facility_Management.My.Resources.ftp)
            CMS_ListSelectClient.Items.Item(CMS_ListSelectClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

            If ListSelectPatch.Items.Count > 0 Then
                CMS_ListSelectClient.Items.Add(cMSDic.Keys(9), Facility_Management.My.Resources.telecharger)
                CMS_ListSelectClient.Items.Item(CMS_ListSelectClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            End If

            CMS_ListSelectClient.Items.Add(cMSDic.Keys(1), Facility_Management.My.Resources.Edit)
            CMS_ListSelectClient.Items.Item(CMS_ListSelectClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            CMS_ListSelectClient.Items.Add(cMSDic.Keys(5), Facility_Management.My.Resources.Supprimer)
            CMS_ListSelectClient.Items.Item(CMS_ListSelectClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        End If

        CMS_ListSelectClient.Items.Add(cMSDic.Keys(41), Facility_Management.My.Resources.import)
        CMS_ListSelectClient.Items.Item(CMS_ListSelectClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        If ListSelectClient.Items.Count > 0 Then
            CMS_ListSelectClient.Items.Add(cMSDic.Keys(36), Facility_Management.My.Resources.document_a_exportation)
            CMS_ListSelectClient.Items.Item(CMS_ListSelectClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            CMS_ListSelectClient.Items.Add(cMSDic.Keys(42), Facility_Management.My.Resources.couper_fichier)
            CMS_ListSelectClient.Items.Item(CMS_ListSelectClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            CMS_ListSelectClient.Items.Add(cMSDic.Keys(6), Facility_Management.My.Resources.SupprimerAll)
            CMS_ListSelectClient.Items.Item(CMS_ListSelectClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        End If

        If client <> Nothing Then
            CMS_ListSelectClient.Items.Add(cMSDic.Keys(14), Facility_Management.My.Resources.Add_package)
            CMS_ListSelectClient.Items.Item(CMS_ListSelectClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        End If

        CMS_ListSelectClient.Items.Add(cMSDic.Keys(21), Facility_Management.My.Resources._Auto)
        CMS_ListSelectClient.Items.Item(CMS_ListSelectClient.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub CMS_ListSelectPatch_Opening(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles CMS_ListSelectPatch.Opening

        CMS_ListSelectPatch.Items.Clear()

        Dim patch As String = CStr(ListSelectPatch.SelectedItem)
        If patch <> Nothing Then

            CMS_ListSelectPatch.Items.Add(cMSDic.Keys(10), Facility_Management.My.Resources.ftp)
            CMS_ListSelectPatch.Items.Item(CMS_ListSelectPatch.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

            If ListSelectClient.Items.Count > 0 Then
                CMS_ListSelectPatch.Items.Add(cMSDic.Keys(9), Facility_Management.My.Resources.telecharger)
                CMS_ListSelectPatch.Items.Item(CMS_ListSelectPatch.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            End If

            CMS_ListSelectPatch.Items.Add(cMSDic.Keys(1), Facility_Management.My.Resources.Edit)
            CMS_ListSelectPatch.Items.Item(CMS_ListSelectPatch.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            CMS_ListSelectPatch.Items.Add(cMSDic.Keys(5), Facility_Management.My.Resources.Supprimer)
            CMS_ListSelectPatch.Items.Item(CMS_ListSelectPatch.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        End If
        CMS_ListSelectPatch.Items.Add(cMSDic.Keys(6), Facility_Management.My.Resources.SupprimerAll)
        CMS_ListSelectPatch.Items.Item(CMS_ListSelectPatch.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub CMS_ListPatch_Opening(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles CMS_ListPatch.Opening

        CMS_ListPatch.Items.Clear()

        '********************************************************************
        '* Pas de création sauf pour Atelier Internet II avec InstallShield *
        '********************************************************************
        'If IO.File.Exists(svAppBuild) Then
        CMS_ListPatch.Items.Add(cMSDic.Keys(0), Facility_Management.My.Resources.Create)
        CMS_ListPatch.Items.Item(CMS_ListPatch.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        'End If

        '********************************************************************

        Dim client As String = CStr(ListPatch.SelectedItem)

        If client <> Nothing Then
            CMS_ListPatch.Items.Add(cMSDic.Keys(3), Facility_Management.My.Resources.Add)
            CMS_ListPatch.Items.Item(CMS_ListPatch.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            CMS_ListPatch.Items.Add(cMSDic.Keys(1), Facility_Management.My.Resources.Edit)
            CMS_ListPatch.Items.Item(CMS_ListPatch.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            CMS_ListPatch.Items.Add(cMSDic.Keys(7), Facility_Management.My.Resources.Preview)
            CMS_ListPatch.Items.Item(CMS_ListPatch.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            CMS_ListPatch.Items.Add(cMSDic.Keys(35), Facility_Management.My.Resources.radar_wifi_icone)
            CMS_ListPatch.Items.Item(CMS_ListPatch.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
            CMS_ListPatch.Items.Add(cMSDic.Keys(2), Facility_Management.My.Resources.corbeille)
            CMS_ListPatch.Items.Item(CMS_ListPatch.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        End If


        CMS_ListPatch.Items.Add(cMSDic.Keys(19), Facility_Management.My.Resources.reload)
        CMS_ListPatch.Items.Item(CMS_ListPatch.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None


        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub Send2FTP(Optional ByVal svOnlySelectedClient As String = Nothing, Optional ByVal svOnlySeclectedPatch As String = Nothing, Optional ByVal MsgFinishVisible As Boolean = True)

        Disabled_FormBox()

        If ListSelectClient.Items.Count = 0 And ListSelectPatch.Items.Count = 0 Then
            If MsgFinishVisible Then
                MsgBox("No selection !", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Transfer FTP Customers and patchs")
            End If
            Enabled_FormBox()
            Exit Sub
        End If

        If MsgFinishVisible Then
            If MsgBox("Are you sure you want to patch the FTP server ?", CType(MsgBoxStyle.DefaultButton2 + MsgBoxStyle.YesNo + MsgBoxStyle.Information, MsgBoxStyle), "Facility Management - Transfer FTP Customers and patchs") = MsgBoxResult.No Then
                Enabled_FormBox()
                Exit Sub
            End If
        End If

        '******************* Olive ********************
        Dim ListPAT = New System.Windows.Forms.ListBox
        If svOnlySeclectedPatch = Nothing Then
            ListPAT = ListSelectPatch
        Else
            ListPAT.Items.Add(svOnlySeclectedPatch)
        End If
        '******************* Olive ********************

        For Each n In ListPAT.Items

            Dim FileName As String = mPathExes + CStr(n)

            Dim CheckSum As String = Append(FileName)

            Dim RemoteFile As String = ParsePath(CStr(n), FILENAME_ONLY) + "." + CheckSum

            If Val(Select_GetIniString(MenuSoft, "UpdateApps", FacilityManagementIni).Trim) = 1 Then

                Dim RemoteAcces As String = ParsePath(CStr(n), FILENAME_ONLY) + ".Update"

                Dim svOptions As String = Nothing

                If MenuSoft = "AutoUpdate" Then

                    'Options
                    If MsgBox("Voulez-vous afficher un message avant de faire la mise à jour pour tout le monde ? ", CType(MsgBoxStyle.DefaultButton2 + MsgBoxStyle.YesNo + MsgBoxStyle.Information, MsgBoxStyle), "Facility Management - Transfer FTP Customers and patchs") = MsgBoxResult.Yes Then
                        svOptions = ";ALL"
                    ElseIf MsgBox("Voulez-vous afficher un message avant de faire la mise à jour pour les machines virtuelles ? ", CType(MsgBoxStyle.DefaultButton2 + MsgBoxStyle.YesNo + MsgBoxStyle.Information, MsgBoxStyle), "Facility Management - Transfer FTP Customers and patchs") = MsgBoxResult.Yes Then
                        svOptions = ";VIRTUAL"
                    Else
                        svOptions = Nothing
                    End If

                End If

                If Val(Select_GetIniString(MenuSoft, "ActiveTesting", FacilityManagementIni).Trim) = 1 Then
                    If MsgBox("Do you want to send for testing ? (" + CStr(n) + ")", CType(MsgBoxStyle.DefaultButton2 + MsgBoxStyle.YesNo + MsgBoxStyle.Information, MsgBoxStyle), "Facility Management - Transfer FTP Customers and patchs") = MsgBoxResult.Yes Then
                        RemoteAcces += "Test"
                    End If
                End If

                Dim AppVersion As String = FileVersionInfo.GetVersionInfo(FileName).FileVersion
                Dim RemoteSize As Double = FileLen(FileName)
                Dim Acces As String = AppVersion + ";" + CStr(RemoteSize).Trim + ";" + CheckSum + svOptions + ";" + BaliseUpdateEndOfFile

                IO.File.WriteAllText(mPathExes + RemoteAcces, Acces, System.Text.Encoding.Default)

                MConnection_UploadFile(mPathExes + RemoteAcces, RemoteAcces)

                IO.File.Delete(mPathExes + RemoteAcces)

            End If

            ToolStripStatusLabelFM.Text = "Transfer FTP: " + CStr(n)

            System.Windows.Forms.Application.DoEvents()

            MConnection_UploadFile(FileName, RemoteFile)

            System.Windows.Forms.Application.DoEvents()

        Next

        '******************* Olive ********************
        Dim ListCLI = New System.Windows.Forms.ListBox
        If svOnlySelectedClient = Nothing Then
            ListCLI = ListSelectClient
        Else
            ListCLI.Items.Add(svOnlySelectedClient)
        End If
        '******************* Olive ********************

        For Each j In ListCLI.Items

            If CheckCRCForClient(j.ToString) Then

                Dim FileName As String = mPathAcces + AliasDic(CStr(j)) + mExtFileAcces

                Dim RemoteFile As String = AliasDic(CStr(j)) + "." + AppendString(Crypter(AliasDic(CStr(j)), "sha1"))

                ToolStripStatusLabelFM.Text = "Transfer FTP: " + CStr(j) + mExtFileAcces

                System.Windows.Forms.Application.DoEvents()

                MConnection_UploadFile(FileName, RemoteFile)

                System.Windows.Forms.Application.DoEvents()

            End If

        Next

        ToolStripStatusLabelFM.Text = "Transfer FTP: completed"

        System.Windows.Forms.Application.DoEvents()

        If MsgFinishVisible Then
            MsgBox("Completed.", CType(MsgBoxStyle.Information + MsgBoxStyle.OkOnly + MsgBoxStyle.SystemModal, MsgBoxStyle))
        End If

        ToolStripStatusLabelFM.Text = Nothing
        ToolStripProgressBar1.Value = 0

        System.Windows.Forms.Application.DoEvents()

        Enabled_FormBox()


    End Sub

    Private Sub CRC2File()

        Disabled_FormBox()

        If ListSelectClient.Items.Count = 0 And ListSelectPatch.Items.Count = 0 Then
            MsgBox("No selection !", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - Creating CRC")
            Enabled_FormBox()
            Exit Sub
        End If

        If MsgBox("Are you sure you want to create CRC file ?", CType(MsgBoxStyle.DefaultButton2 + MsgBoxStyle.YesNo + MsgBoxStyle.Information, MsgBoxStyle), "Facility Management - Creating CRC") = MsgBoxResult.No Then
            Enabled_FormBox()
            Exit Sub
        End If

        Dim lstpatch As ArrayList = New ArrayList
        For Each n In ListSelectPatch.Items
            lstpatch.Add(n)
        Next

        For Each n In lstpatch

            Dim CheckSum As String = Append(mPathExes + CStr(n))
            ListSelectPatch.SelectedItem = n
            System.Windows.Forms.Application.DoEvents()

        Next

        ToolStripStatusLabelFM.Text = "Creating CRC: completed"

        System.Windows.Forms.Application.DoEvents()

        MsgBox("Completed.", CType(MsgBoxStyle.Information + MsgBoxStyle.OkOnly + MsgBoxStyle.SystemModal, MsgBoxStyle))

        ToolStripStatusLabelFM.Text = Nothing
        ToolStripProgressBar1.Value = 0

        System.Windows.Forms.Application.DoEvents()

        Enabled_FormBox()

    End Sub

    Public Function GetFiles(ByVal SourceFolder As String, ByVal Filter As String, ByVal searchOption As System.IO.SearchOption) As String()
        ' ArrayList will hold all file names
        Dim alFiles As ArrayList = New ArrayList()

        ' Create an array of filter string
        Dim MultipleFilters() As String = Filter.Split(CChar("|"))

        ' for each filter find mathing file names
        For Each FileFilter As String In MultipleFilters
            ' add found file names to array list
            alFiles.AddRange(IO.Directory.GetFiles(SourceFolder, FileFilter, searchOption))
        Next

        ' returns string array of relevant file names
        Return CType(alFiles.ToArray(Type.GetType("System.String")), String())
    End Function

    Public Sub CreateListExes()

        ListPatch.Items.Clear()
        ListSelectPatch.Items.Clear()
        DicExes.Clear()

        Dim myDirectory As IO.DirectoryInfo = New IO.DirectoryInfo(mPathExes)
        Dim ListExes As String()

        ListExes = myDirectory.GetFiles("*.exe", IO.SearchOption.TopDirectoryOnly).OrderByDescending(Function(x) x.LastWriteTime).Select(Function(x) x.FullName).ToArray '.Take(40).ToArray

        ListExes = (From w In ListExes Select ParsePath(w, FILENAME)).ToArray()

        For Each obj In ListExes
            ListPatch.Items.Add(obj)
            DicExes.Add(obj, True)
        Next

        Label3.Text = "Selected Updates (" + CStr(ListSelectPatch.Items.Count) + ") :  "
        Label4.Text = "Updates List (" + CStr(ListPatch.Items.Count) + "):"

    End Sub

    Private Sub InvertClient()
        'Invert

        For Each pair As KeyValuePair(Of String, String) In AliasDic

            Dim client As String = pair.Key

            If ListSelectClient.Items.Contains(client) Then
                ListAccesClient.Items.Add(client)
                ListSelectClient.Items.Remove(client)
            Else
                ListSelectClient.Items.Add(client)
                ListAccesClient.Items.Remove(client)
            End If

        Next

    End Sub

    Private Sub ListPatch_MouseDown(sender As Object, e As System.Windows.Forms.MouseEventArgs) Handles ListPatch.MouseDown

        If e.Button = MouseButtons.Right Then
            ListPatch_mDown = True
        End If

    End Sub

    Private Sub ListPatch_MouseLeave(sender As Object, e As System.EventArgs) Handles ListPatch.MouseLeave

        If ListPatch_mDown = False Then
            ListPatch.SelectedIndex = -1
        Else
            ListPatch_mDown = False
        End If

    End Sub

    Private Sub ListPatch_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListPatch.MouseMove

        ToolTipInfo(ListPatch, e, mPathExes, mExtFileInfo)

    End Sub

    Private Sub ListSelectPatch_MouseDown(sender As Object, e As System.Windows.Forms.MouseEventArgs) Handles ListSelectPatch.MouseDown

        If e.Button = MouseButtons.Right Then
            ListSelectPatch_mDown = True
        End If

    End Sub

    Private Sub ListSelectPatch_MouseLeave(sender As Object, e As System.EventArgs) Handles ListSelectPatch.MouseLeave

        If ListSelectPatch_mDown = False Then
            ListSelectPatch.SelectedIndex = -1
        Else
            ListSelectPatch_mDown = False
        End If

    End Sub

    Private Sub ListSelectPatch_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles ListSelectPatch.MouseMove

        ToolTipInfo(ListSelectPatch, e, mPathExes, mExtFileInfo)

    End Sub

    Private Sub SoftwareMenu_DropDownOpening(ByVal sender As Object, ByVal e As System.EventArgs) Handles SoftwareMenu.DropDownOpening

        SoftwareMenu.DropDownItems.Clear()
        For Each u In SoftwareDic.Keys

            For Each s In CountryDic.Values
                If IO.Directory.Exists(SoftwareDic(u) + s) Then

                    Dim SoftImage As Image
                    Try
                        SoftImage = DicSoftImage(u)
                    Catch ex As Exception
                        SoftImage = Facility_Management.My.Resources.avertissement
                    End Try

                    SoftwareMenu.DropDownItems.Add(u, SoftImage)
                    'SoftwareMenu.DropDownItems.Item(SoftwareMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.SizeToFit
                    SoftwareMenu.DropDownItems.Item(SoftwareMenu.DropDownItems.Count - 1).ImageScaling = ToolStripItemImageScaling.None
                    SoftwareMenu.DropDownItems.Item(SoftwareMenu.DropDownItems.Count - 1).BackColor = backcolor2

                    Exit For

                End If
            Next

        Next

        If SoftwareMenu.DropDownItems.Count = 1 Then
            MenuSoft = SoftwareMenu.DropDownItems.Item(0).Text
            SoftwareMenu.DropDownItems.Clear()
        End If

    End Sub

    Private Sub FiltrePatch_KeyDown(sender As Object, e As System.Windows.Forms.KeyEventArgs) Handles FiltrePatch.KeyDown
        If e.KeyCode = Keys.Escape Then
            FiltrePatch.Text = ""
        End If

    End Sub

    Private Sub FiltrePatch_MouseDoubleClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles FiltrePatch.MouseDoubleClick
        If FilterSupp Then
            FiltrePatch.BackColor = System.Drawing.Color.FromKnownColor(KnownColor.Window)
            FiltrePatch.ForeColor = System.Drawing.Color.FromKnownColor(KnownColor.WindowText)
            FilterSupp = False
        Else
            FiltrePatch.BackColor = System.Drawing.Color.FromKnownColor(KnownColor.Red) 'Color.Red
            FiltrePatch.ForeColor = System.Drawing.Color.FromKnownColor(KnownColor.White)
            FilterSupp = True
        End If

        If FiltrePatch.Text <> Nothing Then
            PatchFilter()
        End If

    End Sub

    Private Sub FiltrePatch_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FiltrePatch.TextChanged

        PatchFilter()

    End Sub

    Private Sub PatchFilter()

        Dim str As String = FiltrePatch.Text
        ListPatch.Items.Clear()
        Dim ListKeys(DicExes.Keys.Count - 1) As String
        DicExes.Keys.CopyTo(ListKeys, 0)

        For Each key In ListKeys
            If str <> Nothing Then
                'If InStr(key, str, CompareMethod.Text) > 0 And Not ListSelectPatch.Items.Contains(key) Then
                If CBool(IIf(FilterSupp, (InStr(key, str, CompareMethod.Text) = 0), (InStr(key, str, CompareMethod.Text) > 0))) And Not ListSelectPatch.Items.Contains(key) Then
                    ListPatch.Items.Add(key)
                    DicExes(key) = True
                Else
                    DicExes(key) = False
                End If
            ElseIf Not ListSelectPatch.Items.Contains(key) Then
                ListPatch.Items.Add(key)
                DicExes(key) = True
            Else
                DicExes(key) = False
            End If
        Next

        Label3.Text = "Selected Updates (" + CStr(ListSelectPatch.Items.Count) + "):"
        Label4.Text = "Updates List (" + CStr(ListPatch.Items.Count) + "):"

    End Sub

    Private Sub ButtonFiltreUpdates_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonFiltreUpdates.Click
        FiltrePatch.BackColor = System.Drawing.Color.FromKnownColor(KnownColor.Window)
        FiltrePatch.ForeColor = System.Drawing.Color.FromKnownColor(KnownColor.WindowText)
        FilterSupp = False
        FiltrePatch.Text = Nothing
    End Sub

    Private Function ReadSql() As Dictionary(Of String, String)

        Dim DicPaSo As New Dictionary(Of String, String)

        'Si base SQL non paramètrée
        If SQL_Serveur = "" Then Return DicPaSo

        Dim database As String = Select_GetIniString(MenuSoft, "database", FacilityManagementIni) 'databaseDic(MenuSoft)

        If database <> "" Then
            Try

                Dim ConnectionSql As SqlConnection

                'Chaîne de connexion
                Dim connectString As String = "database=" + database + ";server=" + SQL_Serveur + ";User ID=" + SQL_Login + ";pwd=" + SQL_Password
                'Dim connectString As String = "database=" + database + ";server=jfc-intranet;User ID=client;pwd=client"

                'Objet connection
                ConnectionSql = New SqlConnection(connectString)

                'Ouverture
                ConnectionSql.Open()

                'Dim Count As SqlCommand = New SqlCommand("Select * from Poste where Poste.Poste_Societe = 'GDA' and Poste.Poste_HardwareCode <> '0' and Poste.Poste_Supp = 0", ConnectionSql)

                'Dim readerCount As SqlDataReader = Count.ExecuteReader()

                'Dim rowCount As Object() = New Object(1) {}

                'readerCount.Read()

                'readerCount.GetValues(rowCount)

                'Dim command As SqlCommand = New SqlCommand("Select Societe.Societe_Nom from Pays inner join Societe on Pays.Pays_Id = Societe.Societe_Pays where Pays.Pays_Name = '" + "France" + "'", connection)

                'Dim command As SqlCommand = New SqlCommand("Select Societe.Societe_Nom, Pays.Pays_Name from Pays inner join Societe on Pays.Pays_Id = Societe.Societe_Pays ORDER BY Societe.Societe_Nom ASC", ConnectionSql)

                Dim command As SqlCommand = New SqlCommand("SELECT Societe.Societe_Nom, Pays.Pays_Name from Poste inner join Societe ON Poste.Poste_Societe = Societe.Societe_Id INNER JOIN Pays ON Societe.Societe_Pays = Pays.Pays_Id WHERE Poste.Poste_HardwareCode <> '0' and Poste.Poste_Supp = 0 ORDER BY Societe.Societe_Nom ASC", ConnectionSql)

                '"SELECT * FROM poste WHERE poste_societe = " + cstr(RSSociete("societe_id")) + " AND poste_hardwarecode <> '0' and poste_supp = 0"

                '"SELECT COUNT(*) FROM poste WHERE poste_societe = " + cstr(RSSociete("societe_id")) + " AND poste_hardwarecode <> '0' and poste_supp = 0"

                Dim reader As SqlDataReader = command.ExecuteReader()

                Dim row As Object() = New Object(1) {}

                While reader.Read()

                    reader.GetValues(row)

                    Dim societe As String = CStr(row(0))

                    If Not DicPaSo.ContainsKey(societe) And Not societe.Contains("JFC Test") Then

                        DicPaSo.Add(societe, CStr(row(1)))

                    End If


                End While

                'Fermeture reader
                reader.Close()

                'Fermeture
                ConnectionSql.Close()


            Catch ex As Exception

                If Not SQL_NoErrorMessage Then
                    MsgBox(ex.Message, CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - " & MenuSoft & " - " & MenuPays)
                End If

            End Try

        End If

        Return DicPaSo

    End Function

    Private Function ReadSqlCreate() As Dictionary(Of String, String)

        Dim DicPaSo As New Dictionary(Of String, String)

        'Si base SQL non paramètrée
        If SQL_Serveur = "" Then Return DicPaSo

        Dim database As String = Select_GetIniString(MenuSoft, "database", FacilityManagementIni) 'databaseDic(MenuSoft)
        If database <> "" Then
            Try

                Dim ConnectionSql As SqlConnection

                'Chaîne de connexion
                Dim connectString As String = "database=" + database + ";server=" + SQL_Serveur + ";User ID=" + SQL_Login + ";pwd=" + SQL_Password

                'Objet connection
                ConnectionSql = New SqlConnection(connectString)

                'Ouverture
                ConnectionSql.Open()

                'Dim command As SqlCommand = New SqlCommand("SELECT Societe.Societe_Nom, Pays.Pays_Name from Poste inner join Societe ON Poste.Poste_Societe = Societe.Societe_Id INNER JOIN Pays ON Societe.Societe_Pays = Pays.Pays_Id ORDER BY Societe.Societe_Nom ASC", ConnectionSql)

                'Dim command As SqlCommand = New SqlCommand("SELECT Societe.Societe_Nom, Pays.Pays_Name from Societe INNER JOIN Pays ON Societe.Societe_Pays = Pays.Pays_Id ORDER BY Societe.Societe_Nom ASC", ConnectionSql)

                Dim command As SqlCommand = New SqlCommand("SELECT Societe.Societe_Nom from Societe INNER JOIN Pays ON Societe.Societe_Pays = Pays.Pays_Id where Pays.Pays_Name like '" + MenuPays + "' ORDER BY Societe.Societe_Nom ASC", ConnectionSql)

                Dim reader As SqlDataReader = command.ExecuteReader()

                'Dim row As Object() = New Object(1) {}

                Dim row As Object() = New Object(0) {}

                While reader.Read()

                    reader.GetValues(row)

                    Dim societe As String = CStr(row(0))

                    If Not DicPaSo.ContainsKey(societe) And Not societe.Contains("JFC Test") Then

                        DicPaSo.Add(societe, MenuPays)
                        'DicPaSo.Add(societe, CStr(row(0)))
                        'DicPaSo.Add(societe, CStr(row(1)))

                    End If


                End While

                'Fermeture reader
                reader.Close()

                'Fermeture
                ConnectionSql.Close()

            Catch ex As Exception

                If Not SQL_NoErrorMessage Then
                    MsgBox(ex.Message, CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - " & MenuSoft & " - " & MenuPays)
                End If

            End Try

        End If

        Return DicPaSo

    End Function

    Private Sub Enabled_FormBox()

        'System.Windows.Forms.Application.DoEvents()

        Me.MenuStripFM.Enabled = True

        ListAccesClient.Enabled = True
        ListSelectPatch.Enabled = True
        ListSelectClient.Enabled = True
        ListPatch.Enabled = True
        FiltrePatch.Enabled = True

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub Disabled_FormBox()

        Me.UpdateMenu.HideDropDown()
        Me.ToolsMenu.HideDropDown()
        Me.SoftwareMenu.HideDropDown()
        Me.CountryMenu.HideDropDown()
        Me.CustomerMenu.HideDropDown()
        Me.ExitMenu.HideDropDown()

        'System.Windows.Forms.Application.DoEvents()

        Me.MenuStripFM.Enabled = False
        ListAccesClient.Enabled = False
        ListSelectPatch.Enabled = False
        ListSelectClient.Enabled = False
        ListPatch.Enabled = False
        FiltrePatch.Enabled = False

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub Group_DropDownItemClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles Group.DropDownItemClicked

        CMS_ListAccesClient.Hide()

        Disabled_FormBox()

        Dim str As String = e.ClickedItem.Text

        AddGroup(str)

        Enabled_FormBox()

    End Sub

    Private Sub AddGroup(Optional ByVal sGroup As String = Nothing)

        Dim nvCountGroup As Integer = 1

        Do

            Dim svGroup As String = Select_GetIniString(MenuSoft, "Group" + CStr(nvCountGroup).Trim, FacilityManagementIni).Trim

            If svGroup = Nothing Then Exit Do

            Dim Clients() As String = Split(svGroup, ",")

            If Clients(0) = sGroup Or sGroup = Nothing Then

                For i As Integer = 1 To Clients.Length - 1
                    SelectedClient(Clients(i).Trim)
                Next

                If Clients(0) = sGroup Then Exit Do

            End If

            nvCountGroup += 1

        Loop

    End Sub

    Private Function ReadSqlExt(ByVal svClient As String, ByVal svPatch As String, Optional ByVal Etat As String = "1") As Boolean

        'Si base SQL non paramètrée
        If SQL_SiteInternet_Serveur = "" Or bAdvancedMenu = False Then Return False

        Try

            svClient = AliasDic(svClient)

            svClient = Replace(svClient, "é", "?")
            svClient = Replace(svClient, "è", "?")

            Dim ConnectionSql As SqlConnection

            'Chaîne de connexion

            Dim connectString As String = "database=siteinternet;server=" + SQL_SiteInternet_Serveur + ";User ID=" + SQL_SiteInternet_Login + ";pwd=" + SQL_SiteInternet_Password

            'Objet connection
            ConnectionSql = New SqlConnection(connectString)

            'Ouverture
            ConnectionSql.Open()

            ''date time' like '%2010%' and

            Dim command As SqlCommand = New SqlCommand("Select DISTINCT maj from Log where  login = '" + svClient + "' and  maj like '%" + svPatch + ".exe' and logiciel = '" + MenuSoft + "' and etat = '" + Etat + "'", ConnectionSql)

            'Object datareader
            Dim reader As SqlDataReader = command.ExecuteReader()

            ReadSqlExt = reader.HasRows

            'Fermeture reader
            reader.Close()

            'Fermeture
            ConnectionSql.Close()

        Catch ex As Exception

            ReadSqlExt = False

            If Not SQL_NoErrorMessage Then
                MsgBox(ex.Message, CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - " & MenuSoft & " - " & MenuPays)
            End If

        End Try

    End Function

    Private Sub FormBoxResize()

        On Error Resume Next

        Dim nMax As Integer = Me.Size.Width - 16
        CustomerMenu.Width = CInt(Int(nMax * 0.18))
        UpdateMenu.Width = CInt(Int(nMax * 0.13))
        ToolsMenu.Width = CInt(Int(nMax * 0.13))
        CountryMenu.Width = CInt(Int(nMax * 0.16))
        ExitMenu.Width = CInt(Int(nMax * 0.13))
        SoftwareMenu.Width = nMax - CustomerMenu.Width - UpdateMenu.Width - ToolsMenu.Width - CountryMenu.Width - ExitMenu.Width
        'SoftwareMenu.Width = CInt(Int(nMax * 0.24))

        Me.ToolStripProgressBar1.Size = New System.Drawing.Size(CInt(Int(nMax * 0.2)), 16)

    End Sub

    Private Sub FormBox_Resize(sender As Object, e As System.EventArgs) Handles Me.Resize
        If Me.Visible = Enabled Then FormBoxResize()
    End Sub

    Private Sub F6_Click(sender As System.Object, e As System.EventArgs) Handles F6.Click
        'send
        AddPatchToClient(, , False)

        Send2FTP(, , False)
    End Sub

    Private Sub F5_Click(sender As System.Object, e As System.EventArgs) Handles F5.Click
        'refresh
        Init_Software()
        GetCustomerOfCountry(True)
        'CreateListExes()
        'PatchFilter()
    End Sub

    Private Sub F4_Click(sender As System.Object, e As System.EventArgs) Handles F4.Click
        'edit
        Dim client As String = CStr(ListAccesClient.SelectedItem)

        If client <> Nothing Then
            client = AliasDic(client)
        Else
            client = CStr(ListSelectClient.SelectedItem)
            If client <> Nothing Then
                client = AliasDic(client)
            End If
        End If

        If client <> Nothing Then
            Dim WinPath As String = Environment.GetEnvironmentVariable("windir")
            LaunchAppAndWait(WinPath + "\notepad.exe", mPathAcces + client + mExtFileAcces, ProcessWindowStyle.Normal, NOWAIT)
        End If
    End Sub

    Private Sub F3_Click(sender As System.Object, e As System.EventArgs) Handles F3.Click
        'create
        CreateUpdate.ShowDialog()
    End Sub

    Private Sub F2_Click(sender As System.Object, e As System.EventArgs) Handles F2.Click

        'new
        Dim NewClient As String = InputBox("New Client:").Trim ', , , CMS_ListAccesClient.Bounds.X, CMS_ListAccesClient.Bounds.Y).Trim

        If NewClient <> Nothing Then
            NewClient.Trim()

            AliasCompany(NewClient)

            For Each u In CountryDic
                Dim PathFileAcces As String = mPathExes + u.Value + NewClient + mExtFileAcces
                If IO.File.Exists(PathFileAcces) Then
                    MsgBox("The client '" + NewClient + "' already exists in'" + u.Key + "'.", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.OkOnly, MsgBoxStyle), "Facility Management - New Client")
                    Enabled_FormBox()
                    Exit Sub
                End If
            Next

            Dim svFileAcces As String = mPathAcces + NewClient + mExtFileAcces
            If IO.File.Exists(svFileAcces) Then
                IO.File.Delete(svFileAcces)
            End If

            IO.File.WriteAllText(svFileAcces, AddCRC2String(BaliseAccesEndOfFile), System.Text.Encoding.Default)

            ListAccesClient.Items.Add(NewClient)

        End If

    End Sub

    Private Sub SplitContainer3_MouseDoubleClick(sender As Object, e As System.Windows.Forms.MouseEventArgs) Handles SplitContainer3.MouseDoubleClick
        SplitContainer3.SplitterDistance = CInt(SplitContainer3.Size.Width / 2)
    End Sub

    Private Sub SplitContainer2_MouseDoubleClick(sender As Object, e As System.Windows.Forms.MouseEventArgs) Handles SplitContainer2.MouseDoubleClick
        SplitContainer2.SplitterDistance = CInt(SplitContainer2.Size.Width / 2)
    End Sub

    Private Sub SplitContainer4_MouseDoubleClick(sender As Object, e As System.Windows.Forms.MouseEventArgs) Handles SplitContainer4.MouseDoubleClick
        SplitContainer4.SplitterDistance = CInt(SplitContainer4.Size.Width / 2)
    End Sub

End Class


