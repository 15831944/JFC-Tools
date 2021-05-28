using System;
using System.Net.NetworkInformation;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace JFCUpdateService
{
	public class MacAddress
{
	public static string GetValue(string sMacAddressCurrent = "")
	{
		NetworkInterface[] allNetworkInterfaces = NetworkInterface.GetAllNetworkInterfaces();
		string[] array = new string[1];
		int num = 0;
		if (!Information.IsArray(allNetworkInterfaces))
		{
			return "000000000000";
		}
		NetworkInterface[] array2 = allNetworkInterfaces;
		checked
		{
			foreach (NetworkInterface networkInterface in array2)
			{
				if (networkInterface.NetworkInterfaceType == NetworkInterfaceType.Ethernet)
				{
					byte[] addressBytes = networkInterface.GetPhysicalAddress().GetAddressBytes();
					array = (string[])Utils.CopyArray(array, new string[num + 1 + 1]);
					array[num] = "";
					int num2 = addressBytes.Length - 1;
					for (int j = 0; j <= num2; j++)
					{
						array[num] += addressBytes[j].ToString("X2");
					}
					num++;
				}
			}
			try
			{
				int num3 = 0;
				if ((Operators.CompareString(sMacAddressCurrent, "", TextCompare: false) != 0) & (sMacAddressCurrent.Length == 12))
				{
					int num4 = num - 1;
					for (int k = 0; k <= num4; k++)
					{
						if (Operators.CompareString(sMacAddressCurrent, array[k], TextCompare: false) == 0)
						{
							num3 = k;
							break;
						}
					}
				}
				return array[num3];
			}
			catch (Exception ex)
			{
				ProjectData.SetProjectError(ex);
				Exception ex2 = ex;
				string result = "000000000000";
				ProjectData.ClearProjectError();
				return result;
			}
		}
	}
}
}