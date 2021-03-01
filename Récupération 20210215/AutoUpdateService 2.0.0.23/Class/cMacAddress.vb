Option Strict On
Option Explicit On

Imports System.Net.NetworkInformation

Public Class MacAddress

    'Déclaration : Constante de séparation des digits de l'adresse MAC
    Shared Function GetValue(Optional ByVal sMacAddressCurrent As String = "") As String

        'Déclaration : Collection des Interfaces Réseau
        Dim nics As NetworkInterface() = NetworkInterface.GetAllNetworkInterfaces()
        'Déclaration : Tableau de Chaines de caractères pour les adresses physiques
        Dim sAdress(0) As String
        'Déclaration : Entier représentant le numéro de l'Interface Réseau en cours de traitement
        Dim iAdress As Integer = 0
        'Vérification de la présence d'au moins une interface Réseau Active sur l'ordinateur 
        If Not IsArray(nics) Then Return "000000000000" : Exit Function
        'Enumération des Interfaces Réseau
        For Each adapter As NetworkInterface In nics

            If adapter.NetworkInterfaceType = NetworkInterfaceType.Ethernet Then

                'Déclaration : Objet PhysicalAddress correspondant à l'Interface Réseau en Cours
                Dim address As PhysicalAddress = adapter.GetPhysicalAddress()
                'Déclaration : Tableau d'octets repésentants l'Adresse Physique
                Dim bytes As Byte() = address.GetAddressBytes()
                'Redimentionnement : Tableau de Chaines de caractères pour les adresses physiques
                '                 -> Ajout d'un Element Initialisé à ""
                ReDim Preserve sAdress(iAdress + 1) : sAdress(iAdress) = ""
                'Enumération des Octets de l'adresse
                For i As Integer = 0 To bytes.Length - 1
                    'Conversion d'un octet en Codé Hexa sur Deux Digits
                    sAdress(iAdress) &= bytes(i).ToString("X2")
                    'Ajout d'un Séparateur entre chaque Octet Codé en Hexa sur Deux Digits
                Next
                'Incrémentation : Entier représentant le numéro de l'Interface Réseau en cours de traitement
                iAdress += 1

            End If

        Next
        'Renvoi de la collection d'adresses en format Chaine de caractères
        Try

            ' Modif Olive le 22/10/2009 pour Orange
            Dim nResult As Integer = 0
            If (sMacAddressCurrent <> "") And sMacAddressCurrent.Length = 12 Then
                For i As Integer = 0 To iAdress - 1
                    If sMacAddressCurrent = sAdress(i) Then
                        nResult = i
                        Exit For
                    End If
                Next
            End If
            ' Modif Olive le 22/10/2009 pour Orange

            Return sAdress(nResult)
        Catch ex As Exception
            Return "000000000000"
        End Try
    End Function

End Class



