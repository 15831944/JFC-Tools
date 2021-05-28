using System.Security.Cryptography;
using System.Text;
using Microsoft.VisualBasic.CompilerServices;

namespace JFCUpdateService
{
    [StandardModule]
    internal sealed class mEasyCrypt
    {
        public static string Crypter(string Texte, string type)
        {
            MD5CryptoServiceProvider mD5CryptoServiceProvider = new MD5CryptoServiceProvider();
            SHA1CryptoServiceProvider sHA1CryptoServiceProvider = new SHA1CryptoServiceProvider();
            SHA256Managed sHA256Managed = new SHA256Managed();
            SHA512Managed sHA512Managed = new SHA512Managed();
            RIPEMD160Managed rIPEMD160Managed = new RIPEMD160Managed();
            byte[] arrInput = null;
            byte[] bytes = Encoding.UTF8.GetBytes(Texte);
            switch (type)
            {
                case "md5":
                    arrInput = mD5CryptoServiceProvider.ComputeHash(bytes);
                    mD5CryptoServiceProvider.Clear();
                    break;
                case "sha1":
                    arrInput = sHA1CryptoServiceProvider.ComputeHash(bytes);
                    sHA1CryptoServiceProvider.Clear();
                    break;
                case "sha256":
                    arrInput = sHA256Managed.ComputeHash(bytes);
                    sHA256Managed.Clear();
                    break;
                case "sha512":
                    arrInput = sHA512Managed.ComputeHash(bytes);
                    sHA512Managed.Clear();
                    break;
                case "ripemd160":
                    arrInput = rIPEMD160Managed.ComputeHash(bytes);
                    rIPEMD160Managed.Clear();
                    break;
            }
            return ByteArrayToString(arrInput);
        }

        public static string ByteArrayToString(byte[] arrInput)
        {
            StringBuilder stringBuilder = new StringBuilder(arrInput.Length);
            checked
            {
                int num = arrInput.Length - 1;
                for (int i = 0; i <= num; i++)
                {
                    stringBuilder.Append(arrInput[i].ToString("X2"));
                }
                return stringBuilder.ToString().ToLower();
            }
        }
    }
}