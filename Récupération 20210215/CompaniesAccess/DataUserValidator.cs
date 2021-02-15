using AggregateEditor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using RadioEngine;
using AR7Common;

namespace CompaniesAccess
{
    public class DataUserValidator : IDataCrossedValidator
    {
        private IUserDataManager m_manager;

        
        public DataUserValidator(IUserDataManager _manager)
        {
            this.m_manager = _manager;
        }

        public bool IsValid(RangeIndexModalities _rangeRow, RangeIndexModalities _rangeCol)
        {
            RangeIndexModalities l_range = new RangeIndexModalities();
            l_range.AddValue(_rangeRow);
            l_range.AddValue(_rangeCol.SubRange(new int[] { this.m_manager.Link.IndexesOfDimension[1] }));
            return this.m_manager.IsValidForUser(l_range.GetAllPermutations().FirstOrDefault());
        }
    }
}
