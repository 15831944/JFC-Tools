using System;
using System.Runtime.InteropServices;
using System.Security.Principal;
using JFCUpdateService;

namespace JFCUpdateService
{
	public class AliasAccount
{
	public class ImpersonationException : Exception
	{
		public readonly int win32ErrorNumber;

		public ImpersonationException(int win32ErrorNumber, string msg, string username, string domainname)
			: base(string.Format("Impersonation of {1}\\{0} failed! [{2}] {3}", username, domainname, win32ErrorNumber, msg))
		{
			this.win32ErrorNumber = win32ErrorNumber;
		}
	}

	private string _username;

	private string _password;

	private string _domainname;

	private IntPtr _tokenHandle;

	private IntPtr _dupeTokenHandle;

	private WindowsImpersonationContext _impersonatedUser;

	public string username => _username;

	public string domainname => _domainname;

	public string password => _password;

	public string currentWindowsUsername => WindowsIdentity.GetCurrent().Name;

	public AliasAccount(string username, string password)
	{
		_tokenHandle = new IntPtr(0);
		_dupeTokenHandle = new IntPtr(0);
		string[] array = username.Split('\\');
		if (array.Length > 1)
		{
			_domainname = array[0];
			_username = array[1];
		}
		else
		{
			_username = username;
		}
		_password = password;
	}

	public AliasAccount(string username, string password, string domainname)
	{
		_tokenHandle = new IntPtr(0);
		_dupeTokenHandle = new IntPtr(0);
		_username = username;
		_password = password;
		_domainname = domainname;
	}

	public void BeginImpersonation()
	{
		_tokenHandle = IntPtr.Zero;
		_dupeTokenHandle = IntPtr.Zero;
		if (!LogonUser(ref _username, ref _domainname, ref _password, 9, 3, ref _tokenHandle))
		{
			int lastWin32Error = Marshal.GetLastWin32Error();
			throw new ImpersonationException(lastWin32Error, GetErrorMessage(lastWin32Error), _username, _domainname);
		}
		if (!DuplicateToken(_tokenHandle, 2, ref _dupeTokenHandle))
		{
			int lastWin32Error = Marshal.GetLastWin32Error();
			CloseHandle(_tokenHandle);
			throw new ImpersonationException(lastWin32Error, "Unable to duplicate token!", _username, _domainname);
		}
		WindowsIdentity windowsIdentity = new WindowsIdentity(_dupeTokenHandle);
		_impersonatedUser = windowsIdentity.Impersonate();
	}

	public void EndImpersonation()
	{
		if (_impersonatedUser != null)
		{
			_impersonatedUser.Undo();
			_impersonatedUser = null;
			if (!(_tokenHandle == IntPtr.Zero))
			{
				CloseHandle(_tokenHandle);
			}
			if (!(_dupeTokenHandle == IntPtr.Zero))
			{
				CloseHandle(_dupeTokenHandle);
			}
		}
	}

	[DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
	private static extern bool LogonUser([MarshalAs(UnmanagedType.VBByRefStr)] ref string lpszUsername, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpszDomain, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpszPassword, int dwLogonType, int dwLogonProvider, ref IntPtr phToken);

	[DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
	private static extern bool DuplicateToken(IntPtr ExistingTokenHandle, int SECURITY_IMPERSONATION_LEVEL, ref IntPtr DuplicateTokenHandle);

	[DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
	private static extern bool CloseHandle(IntPtr handle);

	[DllImport("kernel32.dll")]
	private static extern int FormatMessage(int dwFlags, ref IntPtr lpSource, int dwMessageId, int dwLanguageId, ref string lpBuffer, int nSize, ref IntPtr Arguments);

	private string GetErrorMessage(int errorCode)
	{
		int num = 512;
		int num2 = 4096;
		int nSize = 255;
		string lpBuffer = "";
		int dwFlags = 0x100 | num2 | num;
		IntPtr lpSource = IntPtr.Zero;
		IntPtr Arguments = IntPtr.Zero;
		if (FormatMessage(dwFlags, ref lpSource, errorCode, 0, ref lpBuffer, nSize, ref Arguments) == 0)
		{
			throw new Exception("Failed to format message for error code " + errorCode + ". ");
		}
		return lpBuffer;
	}
}
}