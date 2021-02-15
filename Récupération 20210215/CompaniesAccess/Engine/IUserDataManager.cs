using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AR7Common;
using RadioEngine;

namespace CompaniesAccess
{
    public interface IUserDataManager
    {
        DimensionLinked Link { get; }

        ADimensionCreator DimensionCreator { get; }

        List<RangeIndexModalities> Ranges { get; }

        bool CanOnlySelect { get; }

        bool SelectAllOnHeaderClick { get; }

        bool NewModalityAvailable { get; }

        bool IsValidForUser(IndexModalities _indexes);

        void SaveRanges(List<RangeIndexModalities> _ranges);

        void ClearDimensions();
    }
}
