Option Strict On
Option Explicit On

Imports System.Threading
Imports System.ServiceProcess
Imports Microsoft.Win32
Imports System.IO

Module ReStart

    Public Const PATH As Short = 4
    Public Const FILENAME As Short = 3
    Public Const EXTENSION_ONLY As Short = 2
    Public Const FILENAME_ONLY As Short = 1
    Public Const PATH_FILENAME_ONLY As Short = 8

    Sub Main()

        Dim NameService As String ' = "JFC Update" 

        If Command().Trim = Nothing Then

            Console.WriteLine("Name: ")

            NameService = Console.ReadLine()

        Else

            NameService = Command().Trim.Replace(Chr(34), "").Replace("'", "")
        End If

        Console.WriteLine("ReStart: " + NameService)

        Console.WriteLine("Waiting...")
        Thread.Sleep(10000)

        Dim scStop As ServiceController = New ServiceController(NameService)
        If scStop.Status <> ServiceControllerStatus.Stopped Then
            Try
                Console.WriteLine("Stopping...")
                scStop.Stop()
                Console.WriteLine("Stopped.")
                Console.WriteLine("Waiting...")
                Thread.Sleep(10000)
            Catch ex As Exception
                Console.WriteLine(ex.Message)
            End Try
        End If
        scStop.Close()

        Dim scKill As ServiceController = New ServiceController(NameService)
        If scKill.Status <> ServiceControllerStatus.Stopped Then
            Dim imagePath As String = Registry.GetValue("HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\" & NameService, "ImagePath", "").ToString.Replace(Chr(34), "")
            If File.Exists(imagePath) Then
                imagePath = ParsePath(imagePath, FILENAME_ONLY)
                KillProcess(imagePath)
                Console.WriteLine("Waiting...")
                Threading.Thread.Sleep(10000)
            Else
                Console.WriteLine("File not found.")
            End If
        End If
        scKill.Close()

        Dim scStart As ServiceController = New ServiceController(NameService)
        If scStart.Status = ServiceControllerStatus.Stopped Then

            Try
                Console.WriteLine("Starting...")
                scStart.Start()
                Console.WriteLine("Started.")
            Catch ex As Exception
                Console.WriteLine(ex.Message)
            End Try

        Else
            Console.WriteLine("Not stopped.")
        End If
        scStart.Close()

        If Command().Trim = Nothing Then
            Console.WriteLine("Press any key to exit...")

            Console.ReadKey()
        End If

        End

    End Sub

    Public Sub KillProcess(ByVal szNameProcess As String)

        Dim ListProcess As Process() = Process.GetProcesses()

        For Each pProcess As Process In ListProcess
            If pProcess.ProcessName = szNameProcess Then
                Try
                    Console.WriteLine("Killing...")
                    pProcess.Kill()
                    Console.WriteLine("Killed.")
                Catch ex As Exception
                    Console.WriteLine(ex.Message)
                End Try
            End If
        Next

    End Sub

    Public Function ParsePath(ByVal szPath As String, ByVal nOperation As Short) As String

        Dim nPosSlash, nPosPoint, nLenPath As Integer

        nPosPoint = InStrRev(szPath, ".") - 1
        nPosSlash = InStrRev(szPath, "\")
        nLenPath = Len(szPath)

        If nOperation = EXTENSION_ONLY Then
            Return Right(szPath, nLenPath - nPosPoint)
        ElseIf nOperation = FILENAME_ONLY Then
            Return Mid(szPath, nPosSlash + 1, nPosPoint - nPosSlash)
        ElseIf nOperation = FILENAME Then
            Return Right(szPath, nLenPath - nPosSlash)
        ElseIf nOperation = PATH Then
            Return Left(szPath, nPosSlash)
        ElseIf nOperation = PATH_FILENAME_ONLY Then
            Return Left(szPath, nPosPoint)
        Else
            Return szPath
        End If

    End Function

End Module
