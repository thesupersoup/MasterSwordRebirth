#include "sharedutil.h"
#include "statdefs.h"
#include "stats.h"
#include <math.h>
#include <string.h>

statinfo_t NatStatList[6] =
{
	{"Strength"},
	{"Agility"},
	{"Concentration"},
	{"Awareness"},
	{"Fitness"},
	{"Wisdom"},
};

skillstatinfo_t SkillStatList[10] =
{
	{"Swordsmanship", "swordsmanship", STATPROP_TOTAL},
	{"Martial Arts", "martialarts", STATPROP_TOTAL},
	{"Small Arms", "smallarms", STATPROP_TOTAL},
	{"Axe Handling", "axehandling", STATPROP_TOTAL},
	{"Blunt Arms", "bluntarms", STATPROP_TOTAL},
	{"Archery", "archery", STATPROP_TOTAL},
	//	"Shield handling",
	//	"Two-handed weapons",
	//	"Dual weapons",
	{"Spell Casting", "spellcasting", STAT_MAGIC_TOTAL},
	{"Parry", "parry", 1},
	{"Pole Arms", "polearms", STATPROP_TOTAL}, // MiB JUL2010_02 - Pole Arms!
	{"Trade Skills", "noncombat", STAT_NCSKILL_TOTAL}
	//	"Spell Preparation",
	//	"Swimming",
	//	"Pickpocket", true,
};

char* SkillTypeList[3] =
{
	"Proficiency",
	"Balance",
	"Power" 
};

char* SpellTypeList[5] =
{
	"Fire",
	"Ice",
	"Lightning",
	"Divination",
	"Affliction",
};

char* NCSkillList[12] =
{
	"Woodcutting",
	"Mining",
	"Crafting",
	"Blacksmithing",
	"Apothecary",
	"Foraging",
	"Tracking",
	"Fateweaving",
	"Fletching",
	"Thieving",
	"Bardic Lore",
	"Bartering",
};

int GetSkillStatByName(const char* pszName) // Index lookup by name (Skill stats only)
{
	for (int i = 0; i < SKILL_MAX_STATS; i++)
		if (!stricmp(pszName, SkillStatList[i].DllName))
			return SKILL_FIRSTSKILL + i;
	return -1;
}
const char* GetSkillName(int Skill) // Name lookup by index (Any stat)
{
	if (Skill < 0 || Skill >= STATS_TOTAL)
		return "(Invalid Skill)";

	if (Skill < SKILL_FIRSTSKILL)
		return NatStatList[Skill].Name;

	return SkillStatList[Skill - SKILL_FIRSTSKILL].Name;
}
int GetSubSkillByName(const char* pszName)
{
	if (!stricmp(pszName, "prof")) // alias for proficiency
		return 0;
	for (int i = 0; i < STATPROP_TOTAL; i++)
		if (!stricmp(pszName, SkillTypeList[i]))
			return i;
	for (int i = 0; i < STAT_MAGIC_TOTAL; i++)
		if (!stricmp(pszName, SpellTypeList[i]))
			return i;
	for (int i = 0; i < STAT_NCSKILL_TOTAL; i++)
		if (!stricmp(pszName, NCSkillList[i]))
			return i;
	return -1;
}
int GetNatStatByName(const char* pszName)
{
	for (int i = 0; i < NATURAL_MAX_STATS; i++)
		if (!stricmp(pszName, NatStatList[i].Name))
			return i;
	return -1;
}
// Converts stat.prop into valid indices
void GetStatIndices(const char* Name, int& Stat, int& Prop)
{
	msstring FullName = Name;

	msstring StatName = FullName.thru_char(".");
	msstring PropName = FullName.substr(StatName.len() + 1);
	if (StatName.len())
		Stat = GetSkillStatByName(StatName);
	if (PropName.len())
		Prop = GetSubSkillByName(PropName);
}

CSubStat::~CSubStat()
{
}

CSubStat& CSubStat::operator=(const CSubStat& Other)
{
	Value = Other.Value;
	Exp = Other.Exp;
	return *this;
}

int CStat::operator=(int Equals)
{
	int iAdd = int(Equals / (float)m_SubStats.size());
	int iExtra = Equals % m_SubStats.size(), i = 0;

	for (i = 0; i < (signed)m_SubStats.size(); i++)
		m_SubStats[i].Value = iAdd;

	for (iExtra; iExtra > 0; iExtra--)
	{
		int iLowestStat = 0;
		for (i = 0; i < (signed)m_SubStats.size(); i++)
			if (m_SubStats[i].Value < m_SubStats[iLowestStat].Value)
				iLowestStat = i;
		m_SubStats[iLowestStat].Value++;
	}

	return Value();
}

int CStat::operator+=(int Add)
{
	for (Add; abs(Add) > 0; Add -= Add / abs(Add))
	{
		int iLowestStat = 0, i;
		for (i = 0; i < (signed)m_SubStats.size(); i++)
			if (m_SubStats[i].Value < m_SubStats[iLowestStat].Value)
				iLowestStat = i;
		m_SubStats[iLowestStat].Value += Add / abs(Add);
	}
	return Value();
}

int CStat::Value()
{
	int Total = 0;
	int iSubStats = m_SubStats.size();
	for (int i = 0; i < iSubStats; i++)
		Total += m_SubStats[i].Value;

	// (x + floor(y / 2)) / y
	// As long as X is not negative, and Y is >= 2, this will work as expected. It
	// does rounding by adding half of the divisor to the dividend, then dividing 
	// that by the divisor.
	// 
	// Proof:
	// - x=3, y=5: [(3 + (5 / 2)) / 5] -> [(3 + 2) / 5] -> [5 / 5] -> [1]
	// - x=2, y=5: [(2 + (5 / 2)) / 5] -> [(2 + 2) / 5] -> [4 / 5] -> [0]
	// - x=2, y=4: [(2 + (4 / 2)) / 4] -> [(2 + 2) / 4] -> [4 / 4] -> [1]
	// - x=1, y=4: [(1 + (4 / 2)) / 4] -> [(1 + 2) / 4] -> [3 / 4] -> [0]
	// - x=2, y=3: [(2 + (3 / 2)) / 3] -> [(2 + 1) / 3] -> [3 / 3] -> [1]
	// - x=1, y=3: [(1 + (3 / 2)) / 3] -> [(1 + 1) / 3] -> [2 / 3] -> [0]
	// - x=-3, y=-3: [(-3 + (-3 / 2)) / -3] -> [(-3 + -1) / -3] -> [-4 / -3] -> [1]
	// 
	// This breaks if total is negative:
	// - x=-3, y=3: [(-3 + (3 / 2)) / 3] -> [(-3 + 1) / 3] -> [-2 / 3] -> [0] (should be -1)
	// but can be fixed by changing it to: (abs(x) + floor(y / 2)) / y * sign(x)
	int iVal = (Total + (iSubStats / 2)) / iSubStats;
	
	// if value is 0 then return 1, we don't want skills to be less than 1.
	return (iVal == 0) ? 1 : iVal;
}

int CStat::Value(int StatProperty)
{
	if (StatProperty >= (signed)m_SubStats.size())
		return -1;

	return m_SubStats[StatProperty].Value;
}

void CStat::OutDate() // Makes sure an update will be sent next frame
{
	bNeedsUpdate = true;
}

void CStat::Update() // Updates the stat to current - no updates sent
{
	bNeedsUpdate = false;

	for (int i = 0; i < m_SubStats.size(); i++)
	{
		m_SubStats[i].OldValue = m_SubStats[i].Value;
		m_SubStats[i].OldExp = m_SubStats[i].Exp;
	}
}

bool CStat::Changed()
{
	if (bNeedsUpdate)
		return true;

	// If any values changed -> update.
	for (int i = 0; i < m_SubStats.size(); i++)
		if ((m_SubStats[i].Exp != m_SubStats[i].OldExp) || (m_SubStats[i].Value != m_SubStats[i].OldValue))
			return true;

	return false;
}

bool CStat::operator!=(const CStat& Other)
{
	// Just check the substats
	for (int i = 0; i < m_SubStats.size(); i++)
	{
		if (i >= (signed)Other.m_SubStats.size())
			break;
		if (m_SubStats[i].Value != Other.m_SubStats[i].Value)
			return true;
		if (m_SubStats[i].Exp != Other.m_SubStats[i].Exp)
			return true;
	}
	return false;
}

void CStat::InitStatList(statlist& Stats)
{
	Stats.reserve_once(STATS_TOTAL, STATS_TOTAL);
	for (int i = 0; i < STATS_TOTAL; i++)
	{
		msstring_ref Name = (i < NATURAL_MAX_STATS) ? NatStatList[i].Name : SkillStatList[i - NATURAL_MAX_STATS].DllName;
		CStat::skilltype_e Type = (i < NATURAL_MAX_STATS) ? CStat::STAT_NAT : CStat::STAT_SKILL;
		CStat& Stat = Stats[i];
		Stat.m_Name = Name;
		Stat.m_Type = Type;
		int iSubStats = (Stat.m_Type == CStat::STAT_NAT) ? 1 : SkillStatList[i - NATURAL_MAX_STATS].StatCount;
		Stat.m_SubStats.reserve_once(iSubStats, iSubStats);
	}
}
