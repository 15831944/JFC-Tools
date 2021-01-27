Option Strict On
Option Explicit On

Module mCrypter

    Public Function CryptePass(ByVal Pass As String, ByVal Randomize As Short) As String
        Dim cc, aa, bb, dd, FF As String
        Dim e, c, a, b, d, f As Short

        aa = ""
        bb = ""

        c = Randomize
        d = CShort(Int(c / 16))
        e = CShort(c - d * 16)

        For a = 1 To CShort(Len(Pass))
            cc = Hex(Asc((Mid(Pass, a, 1))) Xor c)
            aa += Trim(Hex(CLng(Val("&h" + Mid(cc, 2, 1))) Xor d) + Hex(CLng(Val("&h" + Mid(cc, 1, 1))) Xor e))
        Next

        aa += Hex(c)

        For b = CShort(Len(aa)) To 1 Step -1
            bb += Mid(aa, b, 1)
        Next

        bb = Right$(bb, 1) + Mid(bb, 1, Len(bb) - 1)
        dd = Right$(bb, 1) + Mid(bb, 2, Len(bb) - 2) + Left$(bb, 1)
        dd = Right(dd, CInt(Len(dd) / 2)) + Left(dd, CInt(Len(dd) / 2))

        f = 0
        For b = 1 To CShort(Len(dd)) Step 2
            f = CShort(f + Val("&h" + Mid(dd, b, 2)))
        Next
        FF = Right(Hex(f), 2)
        dd = Right$(FF, 1) + dd + Left$(FF, 1)

        CryptePass = dd



    End Function

End Module