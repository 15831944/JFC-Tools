Option Strict On
Option Explicit On

Module mDecrypter

    Public Function DecryptePass(ByVal Pass As String) As String
        Dim ee, cc, aa, bb, dd, FF As String
        Dim e, c, a, b, d, f As Short

        c = CShort(Len(Pass))

        If Pass <> "" And (c / 2) = Int(c / 2) Then

            aa = ""
            bb = ""
            cc = ""
            dd = ""

            FF = Right(Pass, 1) & Left(Pass, 1)
            ee = Mid(Pass, 2, Len(Pass) - 2)
            f = 0
            For b = 1 To CShort(Len(ee)) Step 2
                f = f + CShort(Val("&h" & Mid(ee, b, 2)))
            Next

            If FF = Right(Hex(f), 2) Then

                dd = Right(ee, CShort(Len(ee) / 2)) & Left(ee, CShort(Len(ee) / 2))
                cc = Right(dd, 1) & Mid(dd, 2, Len(dd) - 2) & Left(dd, 1)
                cc = Mid(cc, 2, Len(cc) - 1) & Left(cc, 1)

                For a = c To 1 Step -1
                    aa = aa & Mid(cc, a, 1)
                Next

                '****** Checksum ******
                c = CShort(Val("&h" & Right(aa, 2)))
                '****** Checksum ******

                d = CShort(Int(c / 16))
                e = CShort(c - d * 16)

                For b = 1 To CShort(Len(aa) - 2) Step 2
                    cc = Trim(Hex(CLng(Val("&h" & Mid(aa, b + 1, 1))) Xor e) & Hex(CLng(Val("&h" & Mid(aa, b, 1))) Xor d))
                    bb = bb & Chr(CInt(CLng(Val("&h" & cc)) Xor c))
                Next

                DecryptePass = bb

            Else
                DecryptePass = ""
            End If
        Else
            DecryptePass = ""
        End If

    End Function

End Module