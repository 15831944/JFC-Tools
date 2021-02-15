Option Strict On
Option Explicit On

Imports System.Threading

Module ReStart


    Sub Main()

        Dim CommandLine_ReStartProg As String = "ReStart"
        Dim CommandLine_ReStartAutoUpdate As String = "RESTART_AUTO_UPDATE"
        Dim CommandLine As String = CStr(IIf(Command$.Contains(CommandLine_ReStartAutoUpdate), Command$.Replace(CommandLine_ReStartAutoUpdate, "").Trim, Command$))

        Dim ProcessName As String = CommandLine

        Dim AppPath As String = System.AppDomain.CurrentDomain.BaseDirectory
        'Dim AppPath As String = IO.Path.GetDirectoryName(Diagnostics.Process.GetCurrentProcess().MainModule.FileName)
        If Right(AppPath, 1) <> "\" Then AppPath = AppPath & "\"

        If IO.File.Exists(AppPath + "ReStart.log") Then
            IO.File.Delete(AppPath + "ReStart.log")
            Thread.Sleep(500)
        End If

        Dim objWriter As New System.IO.StreamWriter(AppPath + "ReStart.log", False, System.Text.Encoding.Default)

        If CommandLine = "" Then
            ProcessName = AppPath & "AU-Serv.EXE"
        Else
            objWriter.Write("Command line : " + CommandLine + vbNewLine)
            Console.WriteLine("Command line : " + CommandLine)
        End If

        Dim nPosPoint As Integer = InStrRev(ProcessName, ".") - 1
        Dim nLenPathFile As Integer = Len(ProcessName)

        Dim Extension As String = Right(ProcessName, nLenPathFile - nPosPoint)

        Dim UpdateFile As String = ProcessName.Replace(Extension, ".Update")
        Dim BackupFile As String = ProcessName.Replace(Extension, ".Backup")

        If IO.File.Exists(UpdateFile) Then

            Console.WriteLine("Updating...")
            objWriter.Write("Updating..." + vbNewLine)

            ' ***** Kill process *****
            If IO.File.Exists(ProcessName) Then
                For Each p As Process In Process.GetProcesses
                    Try
                        If StrComp(ProcessName, p.MainModule.FileName.ToString, CompareMethod.Text) = 0 Then
                            p.Kill()
                            Console.WriteLine("Kill : " + ProcessName)
                            objWriter.Write("Kill : " + ProcessName + vbNewLine)
                            Thread.Sleep(500)
                            Exit For
                        End If
                    Catch ex As Exception
                        'Console.WriteLine("Kill : " + ex.Message)
                    End Try
                Next
            End If

            ' ***** Backup file *****
            If IO.File.Exists(ProcessName) Then
                Try
                    If IO.File.Exists(BackupFile) Then
                        IO.File.Delete(BackupFile)
                        Console.WriteLine("Delete Backup file : " + BackupFile)
                        objWriter.Write("Delete Backup file : " + BackupFile + vbNewLine)
                        Thread.Sleep(500)
                    End If

                    IO.File.Move(ProcessName, BackupFile)
                    Console.WriteLine("Move backup file : " + ProcessName)
                    objWriter.Write("Move backup file : " + ProcessName + vbNewLine)
                    Thread.Sleep(500)
                Catch ex As Exception
                    Console.WriteLine("Backup file : " + ex.Message)
                    objWriter.Write("Backup file : " + ex.Message + vbNewLine)
                End Try
            End If

            ' ***** Delete file *****
            Try
                If IO.File.Exists(ProcessName) Then
                    IO.File.Delete(ProcessName)
                    Console.WriteLine("Delete file : " + ProcessName)
                    objWriter.Write("Delete file : " + ProcessName + vbNewLine)
                    Thread.Sleep(500)
                End If
            Catch ex As Exception
                Console.WriteLine("Delete file : " + ex.Message)
                objWriter.Write("Delete file : " + ex.Message + vbNewLine)
            End Try

            ' ***** Update file *****
            Try
                IO.File.Move(UpdateFile, ProcessName)
                Console.WriteLine("Update file : " + ProcessName)
                objWriter.Write("Update file : " + ProcessName + vbNewLine)
                Thread.Sleep(500)
            Catch ex As Exception
                Console.WriteLine("Update file : " + ex.Message)
                objWriter.Write("Update file : " + ex.Message + vbNewLine)
            End Try


            If IO.File.Exists(ProcessName) Then
                IO.File.Delete(BackupFile)
                Console.WriteLine("ReStart : " + ProcessName)
                objWriter.Write("ReStart : " + ProcessName + vbNewLine)
                Process.Start(ProcessName, CommandLine_ReStartProg)

            ElseIf IO.File.Exists(BackupFile) Then

                Try
                    IO.File.Move(BackupFile, ProcessName)
                    Console.WriteLine("Restore backup file : " + BackupFile)
                    objWriter.Write("Restore backup file : " + BackupFile + vbNewLine)
                    Thread.Sleep(500)
                Catch ex As Exception
                    Console.WriteLine("Restore backup file : " + ex.Message)
                    objWriter.Write("Restore backup file : " + ex.Message + vbNewLine)
                End Try

                If IO.File.Exists(ProcessName) Then
                    Console.WriteLine("Start : " + ProcessName)
                    objWriter.Write("Start : " + ProcessName + vbNewLine)
                    Process.Start(ProcessName)
                Else
                    Console.WriteLine("File not found : " + ProcessName)
                    objWriter.Write("File not found : " + ProcessName + vbNewLine)
                End If

            Else
                Console.WriteLine("File not found : " + ProcessName)
                objWriter.Write("File not found : " + ProcessName + vbNewLine)
            End If

        ElseIf IO.File.Exists(ProcessName) Then

            ' ***** Kill process *****
            If IO.File.Exists(ProcessName) Then
                For Each p As Process In Process.GetProcesses
                    Try
                        If StrComp(ProcessName, p.MainModule.FileName.ToString, CompareMethod.Text) = 0 Then
                            p.Kill()
                            Console.WriteLine("Kill : " + ProcessName)
                            objWriter.Write("Kill : " + ProcessName + vbNewLine)
                            Thread.Sleep(500)
                            Exit For
                        End If
                    Catch ex As Exception
                        'Console.WriteLine("Kill : " + ex.Message)
                    End Try
                Next
            End If

            If IO.File.Exists(BackupFile) Then
                IO.File.Delete(BackupFile)
                Console.WriteLine("Delete Backup file : " + BackupFile)
                objWriter.Write("Delete Backup file : " + BackupFile + vbNewLine)
                Thread.Sleep(500)
            End If

            Console.WriteLine("Start : " + ProcessName)
            objWriter.Write("Start : " + ProcessName + vbNewLine)

            Process.Start(ProcessName, CStr(IIf(Command$.Contains(CommandLine_ReStartAutoUpdate), CommandLine_ReStartAutoUpdate, "")))

        End If

        objWriter.Write("Close")
        objWriter.Close()

        End

    End Sub

End Module
