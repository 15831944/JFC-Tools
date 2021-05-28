// MultiUserEDI.MessageCode
using Microsoft.VisualBasic.CompilerServices;


namespace MultiUserEDI
{
    [StandardModule]
    public sealed class MessageCode
    {
        public const int CODE_PING = 0;

        public const int CODE_DOWNLOADED = 1;

        public const int CODE_CORRECT_EXECUTION = 2;

        public const int CODE_ERROR_UPDATE = 3;

        public const int CODE_UPDATE_STOPED = 4;

        public const int CODE_COPIED = 5;

        public const int CODE_UPLOADED = 6;

        public const int CODE_CORRECT_LICENCE = 7;

        public const int CODE_CLOSE_SERVICE = 8;

        public const int CODE_INSTALLATION = 9;

        public const int CODE_OPEN_SERVICE = 10;

        public const int CODE_SERIAL_INCORRECT = 11;

        public const int CODE_KEY_INVALID = 12;

        public const int CODE_SERIAL_MISSING = 13;

        public const int CODE_LICENSE_LOCKED = 14;

        public const int CODE_LICENSE_EXPIRED = 15;

        public const int CODE_INVALID_SERIAL = 16;

        public const int CODE_INVALID_PARAMETERS = 17;

        public const int CODE_RESET_SERIAL = 18;

        public const int CODE_RESET_LICENCE = 28;

        public const int CODE_INCORRECT_LICENCE_COMPANY = 19;

        public const int CODE_INCORRECT_PARAMETERS = 20;

        public const int CODE_LICENSE_RETURNED = 21;

        public const int CODE_LICENSE_NOT_RETURNED = 22;

        public const int CODE_LICENSE_CHANGED = 28;

        public const int CODE_LICENSE_NOT_CHANGED = 29;

        public const int CODE_EXCEPTION = 26;

        public const int CODE_INITAILIZE_INSTALL = 27;

        public const int CODE_MOVE_EDI = 30;

        public const int CODE_ERROR = 33;

        public const int CODE_LICENSE_NOT_RECOGNIZED = 35;

        public const int CODE_ERROR_ACCES_FILE = 40;

        public const int CODE_DOWNLOADED_ERROR_SIZE = 41;

        public const int CODE_DOWNLOADED_ERROR_CRC = 42;

        public const int CODE_PACKAGE_EXTRACT = 43;

        public const int CODE_PACKAGE_EXTRACT_FAIL = 44;

        public const int CODE_ERROR_CRC_CONFIG_FILE = 45;

        public const int CODE_ERROR_CRC_ACCES_FILE = 46;

        public const int CODE_ADD_CRC_CONFIG_FILE = 47;

        public const int CODE_DOWNLOAD_RESUME = 48;

        public const int CODE_UPDATE_AU_SERV = 50;

        public const int CODE_UPDATE_MULTIUSEREDI = 51;

        public const int CODE_SYNCHRONIZE_UPDATE = 55;

        public const int CODE_PARAMETER_INFO = 69;

        public const int CODE_REDISTRUBUABLE = 70;

        public const int CODE_WIN7RTM = 71;

        public const int CODE_KILLPROCESS = 88;

        public const int CODE_LOCKAPP = 99;
    }
}