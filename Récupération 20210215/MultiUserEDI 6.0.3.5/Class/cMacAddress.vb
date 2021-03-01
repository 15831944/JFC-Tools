Option Strict On
Option Explicit On

Imports System.Net.NetworkInformation

Public Class MacAddress

    Public Shared bVirtual As Boolean

    'Déclaration : Constante de séparation des digits de l'adresse MAC
    Shared Function GetValue(Optional ByVal sMacAddressCurrent As String = Nothing) As String

        'Déclaration : Collection des Interfaces Réseau
        Dim nics As NetworkInterface() = NetworkInterface.GetAllNetworkInterfaces()

        'Déclaration : Tableau de Chaines de caractères pour les adresses physiques
        Dim sAdress(0) As String
        'Déclaration : Entier représentant le numéro de l'Interface Réseau en cours de traitement
        Dim iAdress As Integer = 0
        'Vérification de la présence d'au moins une interface Réseau Active sur l'ordinateur 
        If Not IsArray(nics) Then Return "000000000000" : Exit Function
        'Enumération des Interfaces Réseau

        Dim CheckWireless As Boolean = False

        If sMacAddressCurrent = "000000000000" Then sMacAddressCurrent = Nothing

CheckMacAdress:

        For Each adapter As NetworkInterface In nics

            'If adapter.NetworkInterfaceType = NetworkInterfaceType.Ethernet And (bVirtual Or Not adapter.Description.Contains("Virtual")) Then
            If (adapter.NetworkInterfaceType = NetworkInterfaceType.Ethernet And (bVirtual Or Not adapter.Description.Contains("Virtual")) And Not CheckWireless) Or
                (CheckWireless And adapter.NetworkInterfaceType = NetworkInterfaceType.Wireless80211) Then

                'Déclaration : Objet PhysicalAddress correspondant à l'Interface Réseau en Cours
                Dim address As PhysicalAddress = adapter.GetPhysicalAddress()
                'Déclaration : Tableau d'octets repésentants l'Adresse Physique
                Dim bytes As Byte() = address.GetAddressBytes()
                'Redimentionnement : Tableau de Chaines de caractères pour les adresses physiques
                '                 -> Ajout d'un Element Initialisé à ""
                ReDim Preserve sAdress(iAdress) : sAdress(iAdress) = ""
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

        'Si pas de carte Ethernet alors cherche carte Wifi
        If iAdress = 0 And bVirtual = False And CheckWireless = False Then CheckWireless = True : GoTo checkMacAdress

        'Si carte Ethernet virtuel
        If iAdress = 0 And bVirtual = False Then bVirtual = True : CheckWireless = False : GoTo checkMacAdress

        'Aucune Carte
        If iAdress = 0 Then Return "000000000000" : Exit Function

        'Renvoi de la collection d'adresses en format Chaine de caractères
        Try

            If (sMacAddressCurrent <> Nothing) Then
                If sMacAddressCurrent.Length = 12 Then
                    For Each i As String In sAdress
                        If sMacAddressCurrent.Contains(i) Then Return i : Exit Function
                    Next
                End If
            End If

            Return sAdress(0)

        Catch ex As Exception

            Return "000000000000"

        End Try

    End Function

End Class



