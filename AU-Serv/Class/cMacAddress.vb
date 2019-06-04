Option Strict On
Option Explicit On

'#Const TEST_MAC_ADRESS = True

Imports System.Net.NetworkInformation

Public Class MacAddress

    Structure NETWORK_INTERFACE
        Dim Description As String
        Dim Ethernet As Boolean
        Dim Wireless As Boolean
        Dim MacAddress As String
        Dim Status As Boolean
        Dim Id As String
    End Structure



    Public Shared bVirtual As Boolean
    Public Shared bDebug As Boolean

    'Déclaration : Constante de séparation des digits de l'adresse MAC
    Shared Function GetValue(Optional ByVal sMacAddressCurrent As String = Nothing) As String

#If Not TEST_MAC_ADRESS Then

        'Déclaration : Collection des Interfaces Réseau
        Dim nics As NetworkInterface() = NetworkInterface.GetAllNetworkInterfaces()

        'Déclaration : Tableau de Chaines de caractères pour les adresses physiques
        Dim sAdress(0) As String
        'Vérification de la présence d'au moins une interface Réseau Active sur l'ordinateur 
        If Not IsArray(nics) Then Return "000000000000" : Exit Function
        'Enumération des Interfaces Réseau

        Dim CheckWireless As Boolean = False

        If sMacAddressCurrent = "000000000000" Then sMacAddressCurrent = Nothing

        Dim bExitFunction As Boolean = False

        Dim Network As New ArrayList

        For Each adapter As NetworkInterface In nics

            Try

                Dim NInterface As NETWORK_INTERFACE

                NInterface.Description = adapter.Description

                NInterface.Id = adapter.Id.Replace("{", "").Replace("}", "").Replace("-", "")

                NInterface.Status = (adapter.OperationalStatus = OperationalStatus.Up)

                NInterface.Ethernet = (adapter.NetworkInterfaceType = NetworkInterfaceType.Ethernet)

                NInterface.Wireless = (adapter.NetworkInterfaceType = NetworkInterfaceType.Wireless80211)

                NInterface.MacAddress = adapter.GetPhysicalAddress.ToString

                ''Déclaration : Objet PhysicalAddress correspondant à l'Interface Réseau en Cours
                'Dim address As PhysicalAddress = adapter.GetPhysicalAddress()

                ''Déclaration : Tableau d'octets repésentants l'Adresse Physique
                'Dim bytes As Byte() = address.GetAddressBytes()
                ''Redimentionnement : Tableau de Chaines de caractères pour les adresses physiques
                ''                 -> Ajout d'un Element Initialisé à ""
                'NInterface.MacAddress = Nothing
                ''Enumération des Octets de l'adresse
                'For i As Integer = 0 To bytes.Length - 1
                '    'Conversion d'un octet en Codé Hexa sur Deux Digits
                '    NInterface.MacAddress &= bytes(i).ToString("X2")
                '    'Ajout d'un Séparateur entre chaque Octet Codé en Hexa sur Deux Digits
                'Next

                If sMacAddressCurrent <> Nothing And NInterface.MacAddress = sMacAddressCurrent Then bExitFunction = True

                If Not bDebug And bExitFunction Then Return sMacAddressCurrent : Exit Function

                If bDebug Then

                    Dim theWriter As IO.StreamWriter = IO.File.AppendText(AUService.AppPath + "NetworkInterface.log")

                    theWriter.WriteLine(DateTime.Now.ToString + ";" + adapter.Id + ";" + adapter.Description + ";" + CStr(NetworkTypeInterface(adapter.NetworkInterfaceType)) + ";" + CStr(StatusOperational(adapter.OperationalStatus)) + ";" + adapter.GetPhysicalAddress.ToString + ";" + adapter.Name)

                    theWriter.Close()

                End If

                If NInterface.MacAddress <> "" Then Network.Add(NInterface)

            Catch ex As Exception

                'Finally

            End Try

        Next

        If bDebug And bExitFunction Then Return sMacAddressCurrent : Exit Function

#Else
        sMacAddressCurrent = Nothing

        Dim Network As New ArrayList
        Dim sAdress(0) As String
        Dim TestNetWork As NETWORK_INTERFACE

        Dim ListNetWork() As String = {
        "16/10/2018 10:15:46;{6534F537-F24F-4F12-B44B-74D6C378266C};Microsoft Wi-Fi Direct Virtual Adapter;Wireless80211;Down;36F39A1A8BA0;Connexion au réseau local* 1",
        "16/10/2018 10:15:46;{CF0FB2F0-D429-462C-A361-16412CD8CAB0};Microsoft Wi-Fi Direct Virtual Adapter #2;Wireless80211;Down;34F39A1A8BA1;Connexion au réseau local* 2",
        "16/10/2018 10:15:46;{A50A1D00-F5A4-4E25-A569-D738420E624D};Realtek USB FE Family Controller;Ethernet;Up;A0CEC8C1197F;Ethernet 3",
        "16/10/2018 10:15:46;{151485E7-7E70-4B03-9537-BAE67114BB6A};Intel(R) Dual Band Wireless-AC 8260;Wireless80211;Down;34F39A1A8BA0;Wi-Fi",
        "16/10/2018 10:15:46;{CF455983-9D9A-4F9F-AC5B-FEC175B90DCF};Bluetooth Device (Personal Area Network);Ethernet;Down;34F39A1A8BA4;Connexion réseau Bluetooth",
        "16/10/2018 10:15:46;{A568DE25-0B58-11E8-B5A7-806E6F6E6963};Software Loopback Interface 1;Loopback;Up;;Loopback Pseudo-Interface 1"}

        For Each line As String In ListNetWork
            Dim SplitLine() As String = Split(line, ";")
            TestNetWork.Id = SplitLine(1)
            TestNetWork.Description = SplitLine(2)
            TestNetWork.MacAddress = SplitLine(5)
            TestNetWork.Status = (SplitLine(4) = "Up")
            TestNetWork.Ethernet = (SplitLine(3) = "Ethernet")
            TestNetWork.Wireless = (SplitLine(3) = "Wireless80211")
            If TestNetWork.MacAddress <> "" Then Network.Add(TestNetWork)
        Next

#End If

        'Cherche carte Ethernet connecté
        sAdress = CheckNetwork(Network)

        'Si pas de carte Ethernet alors cherche carte Ethernet non connecté
        If sAdress(0) = Nothing Then sAdress = CheckNetwork(Network, , False)

        'Si pas de carte Ethernet alors cherche carte Wifi connecté
        If sAdress(0) = Nothing Then sAdress = CheckNetwork(Network, True)

        'Si pas de carte Ethernet alors cherche carte Wifi non connecté
        If sAdress(0) = Nothing Then sAdress = CheckNetwork(Network, True, False)

        'Si pas de carte Ethernet alors cherche carte Ethernet virtuel connecté
        If sAdress(0) = Nothing And bVirtual Then sAdress = CheckNetwork(Network, , , True)

        'Aucune Carte trouvé
        If sAdress(0) = Nothing Then Return "000000000000" : Exit Function

        Return sAdress(0)

    End Function

    Shared Function ListValue() As ArrayList

        'Déclaration : Collection des Interfaces Réseau
        Dim nics As NetworkInterface() = NetworkInterface.GetAllNetworkInterfaces()

        'Vérification de la présence d'au moins une interface Réseau Active sur l'ordinateur 
        If Not IsArray(nics) Then Return Nothing : Exit Function

        'Enumération des Interfaces Réseau
        Dim Network As New ArrayList

        For Each adapter As NetworkInterface In nics

            Try

                Dim NInterface As NETWORK_INTERFACE

                NInterface.Description = adapter.Description

                NInterface.Id = adapter.Id.Replace("{", "").Replace("}", "").Replace("-", "")

                NInterface.Status = (adapter.OperationalStatus = OperationalStatus.Up)

                NInterface.Ethernet = (adapter.NetworkInterfaceType = NetworkInterfaceType.Ethernet)

                NInterface.Wireless = (adapter.NetworkInterfaceType = NetworkInterfaceType.Wireless80211)

                NInterface.MacAddress = adapter.GetPhysicalAddress.ToString

                If NInterface.MacAddress <> "" Then Network.Add(NInterface)

            Catch ex As Exception

                'Finally

            End Try

        Next

        Return Network

    End Function

    Shared Function CheckNetwork(ByVal Network As ArrayList, Optional ByVal CheckWireless As Boolean = False, Optional ByVal Status As Boolean = True, Optional ByVal Virtual As Boolean = False) As String()

        Dim sAdress(0) As String
        Dim iAdress As Integer = 0

        For Each adapter As NETWORK_INTERFACE In Network

            If ((adapter.Ethernet And (Virtual Or (Not adapter.Description.Contains("Virtual") And Not adapter.Description.Contains("USB"))) And Not CheckWireless) Or
                (CheckWireless And adapter.Wireless)) And adapter.Status = Status Then

                ReDim Preserve sAdress(iAdress)

                sAdress(iAdress) &= adapter.MacAddress

                iAdress += 1

            End If

        Next

        Return sAdress

    End Function

    Shared Function NetworkTypeInterface(ByVal adaptater As Integer) As String

        Select Case adaptater

            Case 94
                Return "AsymmetricDsl"              'The network interface uses an Asymmetric Digital Subscriber Line (ADSL).
            Case 37
                Return "Atm"                        'The network interface uses asynchronous transfer mode (ATM) for data transmission.
            Case 20
                Return "BasicIsdn"                  'The network interface uses a basic rate interface Integrated Services Digital Network (ISDN) connection. ISDN is a set of standards for data transmission over telephone lines.
            Case 6
                Return "Ethernet"                   'The network interface uses an Ethernet connection. Ethernet is defined in IEEE standard 802.3.
            Case 26
                Return "Ethernet3Megabit"           'The network interface uses an Ethernet 3 megabit/second connection. This version of Ethernet is defined in IETF RFC 895.
            Case 69
                Return "FastEthernetFx"             'The network interface uses a Fast Ethernet connection over optical fiber and provides a data rate of 100 megabits per second. This type of connection is also known as 100Base-FX.
            Case 62
                Return "FastEthernetT"              'The network interface uses a Fast Ethernet connection over twisted pair and provides a data rate of 100 megabits per second. This type of connection is also known as 100Base-T.
            Case 15
                Return "Fddi"                       'The network interface uses a Fiber Distributed Data Interface (FDDI) connection. FDDI is a set of standards for data transmission on fiber optic lines in a local area network.
            Case 48
                Return "GenericModem"               'The network interface uses a modem.
            Case 117
                Return "GigabitEthernet"            'The network interface uses a gigabit Ethernet connection and provides a data rate of 1,000 megabits per second (1 gigabit per second).
            Case 144
                Return "HighPerformanceSerialBus"   'The network interface uses a High Performance Serial Bus.
            Case 114
                Return "IPOverAtm"                  'The network interface uses the Internet Protocol (IP) in combination with asynchronous transfer mode (ATM) for data transmission.
            Case 63
                Return "Isdn"                       'The network interface uses a connection configured for ISDN and the X.25 protocol. X.25 allows computers on public networks to communicate using an intermediary computer.
            Case 24
                Return "Loopback"                   'The network interface is a loopback adapter. Such interfaces are often used for testing; no traffic is sent over the wire.
            Case 143
                Return "MultiRateSymmetricDsl"      'The network interface uses a Multirate Digital Subscriber Line."
            Case 23
                Return "Ppp"                        'The network interface uses a Point-To-Point protocol (PPP) connection. PPP is a protocol for data transmission using a serial device.
            Case 21
                Return "PrimaryIsdn"                'The network interface uses a primary rate interface Integrated Services Digital Network (ISDN) connection. ISDN is a set of standards for data transmission over telephone lines.
            Case 95
                Return "RateAdaptDsl"               'The network interface uses a Rate Adaptive Digital Subscriber Line (RADSL).
            Case 28
                Return "Slip"                       'The network interface uses a Serial Line Internet Protocol (SLIP) connection. SLIP is defined in IETF RFC 1055.
            Case 96
                Return "SymmetricDsl"               'The network interface uses a Symmetric Digital Subscriber Line (SDSL).
            Case 9
                Return "TokenRing"                  'The network interface uses a Token-Ring connection. Token-Ring is defined in IEEE standard 802.5.
            Case 131
                Return "Tunnel"                     'The network interface uses a tunnel connection.
            Case 1
                Return "Unknown"                    'The interface type is not known.
            Case 97
                Return "VeryHighSpeedDsl"           'The network interface uses a Very High Data Rate Digital Subscriber Line (VDSL).
            Case 71
                Return "Wireless80211"              'The network interface uses a wireless LAN connection (IEEE 802.11 standard).
            Case 237
                Return "Wman"                       'The network interface uses a mobile broadband interface for WiMax devices.
            Case 243
                Return "Wwanpp"                     'The network interface uses a mobile broadband interface for GSM-based devices.
            Case 244
                Return "Wwanpp2"                    'The network interface uses a mobile broadband interface for CDMA-based devices.
            Case Else
                Return "Nothing (" + CStr(adaptater) + ")"
        End Select

    End Function


    Shared Function StatusOperational(ByVal status As Integer) As String

        Select status

            Case 5
                Return "Dormant"        'The network interface is not in a condition to transmit data packets; it is waiting for an external event.
            Case 2
                Return "Down"           'The network interface is unable to transmit data packets.
            Case 7
                Return "LowerLayerDown" 'The network interface is unable to transmit data packets because it runs on top of one or more other interfaces, and at least one of these "lower layer" interfaces is down.
            Case 6
                Return "NotPresent"     'The network interface is unable to transmit data packets because of a missing component, typically a hardware component.
            Case 3
                Return "Testing"        'The network interface is running tests.
            Case 4
                Return "Unknown"        'The network interface status is not known.
            Case 1
                Return "Up"             'The network interface is up; it can transmit data packets.
            Case Else
                Return "Nothing (" + CStr(status) + ")"
        End Select

    End Function

End Class



