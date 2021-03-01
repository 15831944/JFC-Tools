using System;
using System.Collections.Generic;

namespace CompaniesAccess
{
    public interface IAnnouncerDataSource
    {
        void Initialize();

        void Open();
        void Close();

        IEnumerable<String> Announcers { get; }

        bool AnnouncerIsValid(String _userName, String _announcerName);

        void UpdateRights(List<Tuple<String, String>> _rights);
    }
}
