Module mControlFormBox

    Public Sub SelectedPatch(Optional ByVal str As String = Nothing)

        Dim bNotOption As Boolean = (str = Nothing)

        If bNotOption Then
            str = CStr(FormBox.ListPatch.SelectedItem)
        End If

        If str <> Nothing Then

            If FormBox.DicExes.ContainsKey(str) Then

                FormBox.DicExes.Item(str) = False

                FormBox.ListSelectPatch.Items.Add(str)

                If bNotOption Then
                    FormBox.ListPatch.Items.RemoveAt(FormBox.ListPatch.SelectedIndex)
                Else
                    FormBox.ListPatch.Items.Remove(str)
                End If

                FormBox.Label4.Text = "Updates List (" + CStr(FormBox.ListPatch.Items.Count) + "):"

                FormBox.Label3.Text = "Selected Updates (" + CStr(FormBox.ListSelectPatch.Items.Count) + "):"

            End If

        End If

    End Sub

    Public Sub UnSelectedPatch()

        Dim str As String = CStr(FormBox.ListSelectPatch.SelectedItem)

        If str <> Nothing Then

            FormBox.ListSelectPatch.Items.RemoveAt(FormBox.ListSelectPatch.SelectedIndex)

            Dim Index As Integer = 0

            For Each z In FormBox.DicExes.Keys
                If FormBox.DicExes(z) Then Index += 1
                If z = str Then FormBox.DicExes(z) = True : Exit For
            Next

            FormBox.ListPatch.Items.Insert(Index, str)

            FormBox.Label4.Text = "Updates List (" + CStr(FormBox.ListPatch.Items.Count) + "):"

            FormBox.Label3.Text = "Selected Updates (" + CStr(FormBox.ListSelectPatch.Items.Count) + "):"

        End If
    End Sub

    Public Sub SelectedClient(Optional ByVal str As String = Nothing)

        Dim bNotOption As Boolean = (str = Nothing)

        If bNotOption Then
            str = CStr(FormBox.ListAccesClient.SelectedItem)
        End If

        If str <> Nothing Then

            If FormBox.ListAccesClient.Items.Contains(str) Then

                FormBox.ListSelectClient.Items.Add(str)

                If bNotOption Then
                    FormBox.ListAccesClient.Items.RemoveAt(FormBox.ListAccesClient.SelectedIndex)
                Else
                    FormBox.ListAccesClient.Items.Remove(str)
                End If

            End If

            FormBox.Label1.Text = "Customer List (" + CStr(FormBox.ListAccesClient.Items.Count) + "):"

            FormBox.Label2.Text = "Selected customers (" + CStr(FormBox.ListSelectClient.Items.Count) + "):"

        End If

    End Sub

    Public Sub ConvAllClients(ByVal listclient As ListBox)

        FormBox.ToolStripProgressBar1.Maximum = listclient.Items.Count 'FormBox.ListAccesClient.Items.Count
        FormBox.ToolStripProgressBar1.Value = 0

        For Each a In listclient.Items 'FormBox.ListAccesClient.Items
            ConvClient(CStr(a))
            FormBox.ToolStripProgressBar1.Value = FormBox.ToolStripProgressBar1.Value + 1
        Next

        FormBox.ToolStripProgressBar1.Value = 0

    End Sub

    Public Sub ConvClient(Optional ByVal str As String = "")

        System.Windows.Forms.Application.DoEvents()

        If str = "" Then
            str = FormBox.AliasDic(CStr(FormBox.ListAccesClient.SelectedItem))
        End If

        Dim DicConvUTF7 As New Dictionary(Of String, String)

        If str <> Nothing Then

            DicConvUTF7.Add("agrï¿½gat", "agrégat")
            DicConvUTF7.Add("Mise ï¿½ jour", "Mise à jour")
            DicConvUTF7.Add("Clï¿½", "Clé")
            DicConvUTF7.Add("annï¿½e", "année")

            DicConvUTF7.Add("ublicitï¿½", "ublicitïé")
            DicConvUTF7.Add("Mï¿½diamï¿½trie", "Médiamétrie")
            DicConvUTF7.Add("bloquï¿½s", "bloqués")
            DicConvUTF7.Add("ï¿½cran", "écran")
            DicConvUTF7.Add("pï¿½riode", "période")
            DicConvUTF7.Add("Lagardï¿½re", "Lagardère")
            DicConvUTF7.Add("Mï¿½tropoles", "Métropoles")
            DicConvUTF7.Add("projetï¿½s", "projetés")
            DicConvUTF7.Add("Rï¿½ouverture", "Réouverture")
            DicConvUTF7.Add("Crï¿½ation", "Création")
            DicConvUTF7.Add("associï¿½", "associé")
            DicConvUTF7.Add("libellï¿½", "libellé")
            DicConvUTF7.Add("rï¿½gle", "règle")
            DicConvUTF7.Add("Sept-Dï¿½c", "Sept-Déc")
            DicConvUTF7.Add("Indï¿½", "Indé")
            DicConvUTF7.Add("1ï¿½", "1€")
            DicConvUTF7.Add("ï¿½tudes", "études")
            DicConvUTF7.Add("60ï¿½ï¿½", "60''")
            DicConvUTF7.Add("ï¿½gie", "égie")
            DicConvUTF7.Add("Opï¿½rations", "Opérations")
            DicConvUTF7.Add("Spï¿½ciales", "Spéciales")
            DicConvUTF7.Add("prï¿½dï¿½finies", "prédéfinies")
            DicConvUTF7.Add("systÃ¨me", "système")

            DicConvUTF7.Add(" Ã  ", " à ")
            DicConvUTF7.Add(" ï¿½ ", " à ")

            DicConvUTF7.Add("spÃ©cifique", "spécifique")
            DicConvUTF7.Add("specifique", "spécifique")
            DicConvUTF7.Add("sp?cifique", "spécifique")
            DicConvUTF7.Add("spï¿½cifique", "spécifique")

            DicConvUTF7.Add("rï¿½seau", "réseau")
            DicConvUTF7.Add("rÃ©seau", "réseau")
            DicConvUTF7.Add("r?seau", "réseau")
            DicConvUTF7.Add("reseau", "réseau")

            DicConvUTF7.Add("Dï¿½cembre", "Décembre")
            DicConvUTF7.Add("Decembre", "Décembre")
            DicConvUTF7.Add("D?cembre", "Décembre")
            DicConvUTF7.Add("DÃ©cembre", "Décembre")
            DicConvUTF7.Add("NNRDécembre", "NNRDecembre")

            DicConvUTF7.Add("AoÃ»t", "Août")
            DicConvUTF7.Add("Aoï¿½t", "Août")
            DicConvUTF7.Add("Ao?t", "Août")
            DicConvUTF7.Add("Aout", "Août")
            DicConvUTF7.Add("NNRAoût", "NNRAout")

            DicConvUTF7.Add("FÃ©vrier", "Février")
            DicConvUTF7.Add("Fï¿½vrier", "Février")
            DicConvUTF7.Add("Fevrier", "Février")
            DicConvUTF7.Add("F?vrier", "Février")
            DicConvUTF7.Add("NNRFévrier", "NNRFevrier")
            DicConvUTF7.Add("PopNatFévrier", "PopNatFevrier")

            DicConvUTF7.Add("onnï¿½e", "onnée")
            DicConvUTF7.Add("onnÃ©e", "onnée")
            DicConvUTF7.Add("onn?e", "onnée")
            DicConvUTF7.Add("onnee ", "onnée")


            DicConvUTF7.Add("Ã©dia", "édia")
            DicConvUTF7.Add("?dia", "édia")
            DicConvUTF7.Add("ï¿½dia", "édia")
            'DicConvUTF7.Add("edia", "édia")

            DicConvUTF7.Add("?Version", "Version")

            Dim fileAcces As String = FormBox.mPathAcces + str + FormBox.mExtFileAcces

            '*********************************************************************************************
            'rentre en conflit avec le nom des patchs
            '*********************************************************************************************
            'Dim contents As String = IO.File.ReadAllText(fileAcces, System.Text.Encoding.Default)
            'For Each z In DicConvUTF7.Keys
            '    contents = Replace(contents, z, DicConvUTF7(z))
            'Next
            'IO.File.Delete(fileAcces)
            'IO.File.WriteAllText(fileAcces, contents, System.Text.Encoding.Default)
            '*********************************************************************************************

            Dim contents() As String = IO.File.ReadAllLines(fileAcces, System.Text.Encoding.Default)

            For Each z In DicConvUTF7.Keys

                For x = 0 To contents.Length - 1

                    Dim npos As Integer = InStr(contents(x), ".exe,", CompareMethod.Text)

                    If npos > 0 Then
                        contents(x) = Mid(contents(x), 1, npos - 1) + Replace(contents(x), z, DicConvUTF7(z), npos)
                    End If

                Next

            Next

            IO.File.Delete(fileAcces)

            IO.File.WriteAllLines(fileAcces, contents, System.Text.Encoding.Default)

        End If

    End Sub

    Public Sub UnSelectedClient()

        Dim str As String = CStr(FormBox.ListSelectClient.SelectedItem)

        If str <> Nothing Then
            FormBox.ListAccesClient.Items.Add(str)
            FormBox.ListSelectClient.Items.RemoveAt(FormBox.ListSelectClient.SelectedIndex)

            FormBox.Label1.Text = "Customer List (" + CStr(FormBox.ListAccesClient.Items.Count) + "):"

            FormBox.Label2.Text = "Selected customers (" + CStr(FormBox.ListSelectClient.Items.Count) + "):"

        End If

    End Sub

    Public Sub UnSelectedAllClient()

        For Each a In FormBox.ListSelectClient.Items
            FormBox.ListAccesClient.Items.Add(a)
        Next

        FormBox.ListSelectClient.Items.Clear()

    End Sub

    Public Sub SelectedAllClient()

        For Each a In FormBox.ListAccesClient.Items
            FormBox.ListSelectClient.Items.Add(a)
        Next

        FormBox.ListAccesClient.Items.Clear()

    End Sub

    Public Sub UnSelectedAllPatch()

        FormBox.ListPatch.Items.Clear()
        FormBox.ListSelectPatch.Items.Clear()
        Dim ListKeys(FormBox.DicExes.Keys.Count - 1) As String
        FormBox.DicExes.Keys.CopyTo(ListKeys, 0)

        Dim str As String = FormBox.FiltrePatch.Text

        For Each key In ListKeys
            If str <> Nothing Then
                If InStr(key, str, CompareMethod.Text) > 0 Then
                    FormBox.ListPatch.Items.Add(key)
                    FormBox.DicExes(key) = True
                Else
                    FormBox.DicExes(key) = False
                End If
            Else
                FormBox.ListPatch.Items.Add(key)
                FormBox.DicExes(key) = True
            End If
        Next

        FormBox.Label4.Text = "Updates List (" + CStr(FormBox.ListPatch.Items.Count) + "):"
        FormBox.Label3.Text = "Selected Updates (" + CStr(FormBox.ListSelectPatch.Items.Count) + "):"

    End Sub


End Module
