#include <d2tweaks/client/modules/autosort/autosort_client.h>

#include <d2tweaks/client/client.h>

#include <spdlog/spdlog.h>


#include <d2tweaks/common/common.h>
#include <d2tweaks/common/protocol.h>
#include <d2tweaks/common/asset_manager.h>

#include <d2tweaks/ui/menu.h>
#include <d2tweaks/ui/ui_manager.h>
#include <d2tweaks/ui/controls/control.h>
#include <d2tweaks/ui/controls/button.h>

#include <diablo2/d2common.h>
#include <diablo2/d2client.h>
#include <diablo2/d2win.h>
#include <diablo2/d2gfx.h>
#include <diablo2/d2cmp.h>


#include <diablo2/structures/unit.h>
#include <diablo2/structures/inventory.h>
#include <diablo2/structures/item_data.h>
#include <diablo2/structures/player_data.h>

#include <diablo2/structures/path.h>
#include <diablo2/structures/game.h>
#include <diablo2/structures/data/items_line.h>
#include <diablo2/structures/data/item_types_line.h>

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <time.h>
#include <cmath>
#include <random>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>



#include <DllNotify.h>
#include <D2Template.h>

MODULE_INIT(autosort)

enum ColorEnum {
	RED = diablo2::ui_color_t::UI_COLOR_RED,
	LIGHT_GREEN = diablo2::ui_color_t::UI_COLOR_LIGHT_GREEN,
	BLUE = diablo2::ui_color_t::UI_COLOR_BLUE,
	DARK_GOLD = diablo2::ui_color_t::UI_COLOR_DARK_GOLD,
	GREY = diablo2::ui_color_t::UI_COLOR_GREY,
	BLACK = diablo2::ui_color_t::UI_COLOR_BLACK,
	GOLD = diablo2::ui_color_t::UI_COLOR_GOLD,
	ORANGE = diablo2::ui_color_t::UI_COLOR_ORANGE,
	YELLOW = diablo2::ui_color_t::UI_COLOR_YELLOW,
	DARK_GREEN = diablo2::ui_color_t::UI_COLOR_DARK_GREEN,
	PURPLE = diablo2::ui_color_t::UI_COLOR_PURPLE,
	GREEN = diablo2::ui_color_t::UI_COLOR_GREEN,
	WHITE = diablo2::ui_color_t::UI_COLOR_WHITE,
	BLACK2 = diablo2::ui_color_t::UI_COLOR_BLACK2,
	DARK_WHITE = diablo2::ui_color_t::UI_COLOR_DARK_WHITE,
	LIGHT_GREY = diablo2::ui_color_t::UI_COLOR_LIGHT_GREY
};

class inventory_sort_menu : public d2_tweaks::ui::menu {
	d2_tweaks::common::asset* m_buttons_img;

	d2_tweaks::ui::controls::button* m_sort_inventory_btn;
	d2_tweaks::ui::controls::button* m_sort_stash_btn;
	d2_tweaks::ui::controls::button* m_sort_cube_btn;
public:
	inventory_sort_menu() {
		menu::set_enabled(true);
		menu::set_visible(true);

		//load_xml("d2tweaks\\interfaces\\autosort.xml");
		if (DLLBASE_D2EXPRES != 0)
			load_xml("d2tweaks\\interface_d2expres\\autosort.xml");
		if (DLLBASE_SGD2FREERES != 0)
			load_xml("d2tweaks\\interface_sgd2freeres\\autosort.xml");
		if (DLLBASE_SGD2FREERES == 0 && DLLBASE_D2EXPRES == 0)
			load_xml("d2tweaks\\interface_vanilla\\autosort.xml");

		m_buttons_img = singleton<d2_tweaks::common::asset_manager>::instance().get_mpq_file("d2tweaks\\assets\\buttons", d2_tweaks::common::MPQ_FILE_TYPE_DC6);

		m_sort_inventory_btn = get_button("m_sort_inventory_btn", std::bind(&inventory_sort_menu::sort_inventory_click, this));
		m_sort_stash_btn = get_button("m_sort_stash_btn", std::bind(&inventory_sort_menu::sort_stash_click, this));
		m_sort_cube_btn = get_button("m_sort_cube_btn", std::bind(&inventory_sort_menu::sort_cube_click, this));
	}





	int frame = 0;
	long nEndTime = 0;
	int nTip = 0;
	long DURATION = 60000;
	std::vector<std::wstring> diablo2Tips = {
		L"Always carry a Town Portal scroll for quick escape",
		L"Identify unidentified items before selling them for better prices",
		L"Remember to repair your gear to avoid breaking mid-battle",
		L"Explore the entire map to find hidden treasures and quests",
		L"Stock up on health and mana potions before entering dangerous areas"
		L"Tip: Manage your resources wisely for survival",
		L"Tip: Always carry health potions for emergencies",
		L"Tip: Save high-quality gear for tough boss battles",
		L"Tip: Explore thoroughly to find hidden treasures",
		L"Tip: Customize your gear to adapt to different challenges",
		L"Tip: Utilize hirelings strategically for support in combat",
		L"Tip: Master the art of inventory management for efficiency",
		L"Tip: Save and invest your gold wisely in crucial items",
		L"Tip: Learn and use underused skills for surprising advantages",
		L"Tip: Keep track of waypoints for effective navigation",
		L"Tip: Be cautious with 'Players X' command to avoid overwhelming challenges",
		L"Tip: Always have an escape plan when facing overwhelming odds",
		L"Tip: Experiment with different weapon types for diverse combat strategies",
		L"Tip: Use potions and buffs strategically during boss fights",
		L"Tip: Pay attention to monster modifiers and adjust your tactics accordingly",
		L"Tip: Explore side areas and dungeons for valuable loot and experience",
		L"Tip: Keep a backup set of gear for emergencies and special encounters",
		L"Tip: Stay updated with mod updates and patch notes for new features",
		L"Tip: Join online communities to learn advanced strategies and tactics",
		L"Tip: Balance offense and defense to survive challenging encounters",
		L"Tip: Invest in skills that complement your playstyle and gear setup",
		L"Tip: Use terrain and environment to your advantage during battles",
		L"Tip: Stay patient and persistent, progress in Ironman takes time",
		L"Tip: Don't rush into unknown areas, scout and plan your approach",
		L"Tip: Prioritize resistances and elemental defenses for survival",
		L"Tip: Learn monster behaviors and weaknesses for efficient farming",
		L"Tip: Keep track of important quest items and their locations",
		L"Tip: Experiment with different character builds for diverse gameplay experiences",
		L"Tip: Use traps, potions, and scrolls wisely to gain advantages in battles",
		L"Tip: Take breaks and restock supplies regularly to avoid burnout",
		L"Tip: Share knowledge and strategies with fellow players for mutual benefit",
		L"Tip: Explore lore and backstory for immersive gameplay experiences",
		L"Tip: Don't underestimate common enemies, they can overwhelm careless adventurers",
		L"Tip: Engage in boss fights with a well-thought-out strategy and preparation",
		L"Tip: Complete side quests and challenges for unique rewards and achievements",
		L"Tip: Communicate and coordinate with party members for efficient teamwork",
		L"Tip: Utilize environmental hazards and traps against powerful foes",
		L"Tip: Experiment with different crafting recipes for custom gear upgrades",
		L"Tip: Keep an eye on your surroundings, hidden secrets can lead to great rewards",
		L"Tip: Upgrade your gear regularly to match increasing enemy difficulty",
		L"Tip: Explore alternative routes and paths for unique encounters and rewards",
		L"Tip: Utilize crowd control skills and tactics for managing large groups of enemies",
		L"Tip: Stay aware of game mechanics changes in patches and updates for optimal gameplay",
		L"Tip: Take advantage of synergies between skills and equipment for powerful combos",
		L"Tip: Stay organized with your inventory and stash for efficient item management",
		L"Tip: Engage in player-run events and competitions for added fun and challenges",
		L"Tip: Experiment with different playstyles to keep the game experience fresh and exciting",
		L"Tip: Use consumables wisely, hoarding or overspending can lead to challenges",
		L"Tip: Explore and interact with NPCs for quests, lore, and unique items",
		L"Tip: Plan your character's progression and build for long-term success",
		L"Tip: Master positioning to avoid being overwhelmed by enemies",
		L"Tip: Utilize map awareness for efficient navigation and objective location",
		L"Tip: Upgrade weapons with socketed gems for added elemental damage or bonuses",
		L"Tip: Save unique items for tough encounters or emergencies",
		L"Tip: Invest in resistances against common elemental attacks",
		L"Tip: Upgrade hireling's gear to match your level and increase survivability",
		L"Tip: Practice resource management for efficient use of skills and potions",
		L"Tip: Experiment with skill combinations for versatile combat strategies",
		L"Tip: Use environment obstacles to control enemy movement",
		L"Tip: Upgrade movement speed for faster traversal and escapes",
		L"Tip: Utilize teleport skills for quick navigation and evasion",
		L"Tip: Invest in mana or energy shield for magic defense",
		L"Tip: Customize hotkeys for quick access to essential abilities",
		L"Tip: Upgrade hireling's weapon with gems or runes for increased damage",
		L"Tip: Master dodges and evasive maneuvers to minimize damage",
		L"Tip: Use area-of-effect skills to efficiently clear groups of enemies",
		L"Tip: Upgrade armor with socketed gems for added resistances",
		L"Tip: Explore dungeons fully for valuable rewards and experience",
		L"Tip: Invest in crowd control skills for tactical advantages",
		L"Tip: Engage in hit-and-run tactics to avoid enemy retaliation",
		L"Tip: Upgrade hireling's armor for increased survivability",
		L"Tip: Use town portal scrolls strategically for retreat or restock",
		L"Tip: Invest in skills or items for life regeneration or healing",
		L"Tip: Customize gear to counter specific enemy types or bosses",
		L"Tip: Upgrade hireling's skills to complement your abilities",
		L"Tip: Master the use of consumables for status ailment counters",
		L"Tip: Utilize knockback effects to control enemy engagements",
		L"Tip: Upgrade weapons with ethereal runes or gems for increased damage",
		L"Tip: Invest in critical strike chance or damage for burst potential",
		L"Tip: Customize gear loadout for challenging encounters or areas",
		L"Tip: Upgrade hireling's gear with life steal or leech effects",
		L"Tip: Utilize skills with teleport or dash abilities for quick escapes",
		L"Tip: Invest in mana regeneration for sustained casting",
		L"Tip: Customize skills for synergies and optimal combat",
		L"Tip: Upgrade hireling's gear with magic find bonuses for loot",
		L"Tip: Master crowd control to isolate dangerous enemies",
		L"Tip: Utilize elemental damage conversions against enemy weaknesses",
		L"Tip: Upgrade resistances for added protection",
		L"Tip: Invest in damage reflection or thorn effects for passive damage",
		L"Tip: Customize gear for increased rare and unique drops",
		L"Tip: Upgrade hireling's gear against common enemy attacks",
		L"Tip: Utilize skills with area damage effects efficiently",
		L"Tip: Invest in movement speed boosts for exploration",
		L"Tip: Customize build with synergistic skills and gear",
		L"Tip: Upgrade hireling's weapon against enemy vulnerabilities",
		L"Tip: Utilize skills with fear or taunt effects for control",
		L"Tip: Invest in bonuses against undead, demons, or specific monsters",
		L"Tip: Customize gear with life steal or healing for survivability",
		L"Tip: Upgrade hireling's armor with magic find bonuses",
		L"Tip: Utilize cooldown reduction effects for skill usage",
		L"Tip: Invest in knockback or knockdown effects for control",
		L"Tip: Customize attributes for offense, defense, and utility balance",
		L"Tip: Create backup characters in case of unexpected deaths",
		L"Tip: Prioritize resistances and defensive stats over offensive stats",
		L"Tip: Study monster abilities to anticipate dangerous situations",
		L"Tip: Use crowd control skills to manage overwhelming enemy groups",
		L"Tip: Always carry health and mana potions for emergencies",
		L"Tip: Explore different skill combinations for versatility in combat",
		L"Tip: Pay attention to synergies between skills for enhanced effectiveness",
		L"Tip: Learn boss mechanics to maximize chances of survival",
		L"Tip: Keep an eye on your surroundings for potential dangers",
		L"Tip: Use environmental objects to your advantage in battles",
		L"Tip: Join multiplayer games for added safety and teamwork",
		L"Tip: Consult online guides and forums for valuable gameplay strategies",
		L"Tip: Trade wisely to acquire necessary gear without risking too much",
		L"Tip: Communicate with other players to coordinate actions and strategies",
		L"Tip: Don't rush through areas; proceed cautiously to avoid traps and ambushes",
		L"Tip: Maintain a balance between offense and defense in your character build",
		L"Tip: Take breaks to avoid fatigue and maintain focus during long gaming sessions",
		L"Tip: Backup important save files regularly to prevent data loss",
		L"Tip: Experiment with different playstyles to keep the game experience fresh",
		L"Tip: Stay updated with game patches and updates for bug fixes and new content",
		L"Tip: Have fun and enjoy the challenge of hardcore gameplay!",
		L"Tip: Join online communities to connect with other hardcore players",
		L"Tip: Share your experiences and learn from others to improve your gameplay",
		L"Tip: Customize your key bindings for comfortable and efficient gameplay",
		L"Tip: Invest in quality gaming peripherals for better control and responsiveness",
		L"Tip: Stay calm and focused during intense battles to make better decisions",
		L"Tip: Analyze your deaths to learn from mistakes and avoid repeating them",
		L"Tip: Adjust game settings for optimal performance on your system",
		L"Tip: Organize your inventory for quick access to important items",
		L"Tip: Follow a leveling guide for efficient progression in the game",
		L"Tip: Upgrade your gear regularly to keep up with increasing difficulty",
		L"Tip: Use sound cues to detect hidden or off-screen enemies",
		L"Tip: Minimize distractions during gameplay to stay focused",
		L"Tip: Backup your character data externally for added security",
		L"Tip: Join or form a hardcore gaming clan for support and camaraderie",
		L"Tip: Practice kiting and positioning techniques for survival in tough battles",
		L"Tip: Keep track of cooldowns and timers for effective skill management",
		L"Tip: Explore hidden areas and dungeons for valuable loot and experience",
		L"Tip: Use hotkeys for quick access to commonly used skills and items",
		L"Tip: Learn about monster immunities to avoid ineffective attacks",
		L"Tip: Backup your internet connection with a reliable backup plan for uninterrupted gameplay",
		L"Tip: Avoid risky shortcuts or exploits that may compromise your character",
		L"Tip: Pay attention to loot filters to focus on valuable drops",
		L"Tip: Use the town portal strategically to retreat and recover",
		L"Tip: Customize your user interface for better visibility and efficiency",
		L"Tip: Participate in in-game events for unique rewards and challenges",
		L"Tip: Keep your gaming environment comfortable and ergonomic for extended play sessions",
		L"Tip: Practice patience and persistence; progress may require multiple attempts",
		L"Tip: Take advantage of seasonal or temporary boosts to progress faster",
		L"Tip: Use the in-game map to plan routes and avoid getting lost",
		L"Tip: Invest in life leech or mana leech abilities for sustain during battles",
		L"Tip: Keep an eye on your experience bar to time level-up decisions strategically",
		L"Tip: Consult with experienced players for advanced gameplay tips and strategies",
		L"Tip: Learn about monster spawn locations and behaviors for efficient farming",
		L"Tip: Use debuffs and status effects to weaken tough enemies",
		L"Tip: Manage your gold wisely; spend it on essential upgrades and items",
		L"Tip: Utilize mercenary NPCs for additional firepower and support",
		L"Tip: Experiment with different character builds to find your preferred playstyle",
		L"Tip: Learn about item properties and affixes to identify valuable gear",
		L"Tip: Stay informed about game events and promotions for special rewards",
		L"Tip: Take breaks and stretch regularly to avoid muscle strain during long gaming sessions",
		L"Tip: Backup your game settings and configurations for quick recovery after updates",
		L"Tip: Use defensive skills and abilities proactively to avoid damage spikes",
		L"Tip: Participate in community challenges and contests for fun and rewards",
		L"Tip: Experiment with different weapon types and combinations for varied combat styles",
		L"Tip: Use consumables like scrolls and potions strategically in tough situations",
		L"Tip: Stay aware of in-game events and timers for limited-time opportunities",
		L"Tip: Use the game's forums and social media to connect with fellow players",
		L"Tip: Adjust your gameplay pace based on your character's survivability and damage output",
		L"Tip: Stay positive and learn from failures to improve your overall gameplay",
		L"Tip: Upgrade your storage space for easier item management and organization",
		L"Tip: Watch gameplay tutorials and streams to pick up advanced strategies and techniques",
		L"Tip: Use defensive positioning and movement to avoid unnecessary damage",
		L"Tip: Invest in magic find gear for better chances of finding rare items",
		L"Tip: Keep track of your quest progress to avoid missing important objectives",
		L"Tip: Test your build against different enemy types to ensure overall effectiveness",
		L"Tip: Use the shared stash wisely to transfer items between characters",
		L"Tip: Save valuable crafting materials for high-level item upgrades",
		L"Tip: Maintain a positive relationship with other players for a better gaming experience",
		L"Tip: Use environmental clues and hints to uncover hidden secrets and treasures",
		L"Tip: Invest in movement speed enhancements for faster exploration and travel",
		L"Tip: Customize your chat filters to focus on important in-game communications",
		L"Tip: Join public games for increased social interactions and gameplay variety",
		L"Tip: Support fellow players by sharing knowledge and resources",
		L"Tip: Use boss-specific strategies and tactics for smoother encounters",
		L"Tip: Maintain a backup copy of your gaming preferences and configurations",
		L"Tip: Learn about different damage types to optimize your character's defenses",
		L"Tip: Use the minimap to navigate efficiently in large areas and dungeons",
		L"Tip: Avoid overextending in dangerous areas; retreat strategically when necessary",
		L"Tip: Share your gaming achievements and milestones with the community",
		L"Tip: Experiment with team compositions for multiplayer synergy and success",
		L"Tip: Use voice chat for faster and clearer communication during group activities",
		L"Tip: Set achievable goals for each gaming session to track progress",
		L"Tip: Keep track of important NPC locations for quests and services",
		L"Tip: Stay updated on game economy trends for better trading decisions",
		L"Tip: Use third-party tools responsibly to enhance gameplay without exploits",
		L"Tip: Support game developers by providing constructive feedback and bug reports",
		L"Tip: Take time to appreciate the game's lore and story elements for immersion",
		L"Tip: Use in-game events and festivals to earn unique rewards and items",
		L"Tip: Allocate skill points strategically to match your character's playstyle",
		L"Tip: Save powerful consumables for tough boss encounters and critical moments",
		L"Tip: Explore hidden areas and dungeons to discover rare loot and secrets",
		L"Tip: Use crowd control abilities to manage groups of enemies effectively",
		L"Tip: Upgrade your gear gradually to maintain a balanced power level",
		L"Tip: Save rare crafting materials for high-tier item enhancements",
		L"Tip: Adapt your tactics to different enemy resistances and vulnerabilities",
		L"Tip: Keep an eye on your character's stamina to avoid exhaustion during battles",
		L"Tip: Utilize area-of-effect skills to handle large groups of monsters efficiently",
		L"Tip: Explore diverse terrains and regions for varied monster encounters and challenges",
		L"Tip: Learn about monster behaviors and attack patterns to anticipate their moves",
		L"Tip: Prioritize resistances and defenses based on the areas you are exploring",
		L"Tip: Save quest rewards for moments when they can provide the most benefit",
		L"Tip: Experiment with different weapon and armor combinations for optimal synergies",
		L"Tip: Maintain a balanced approach between offense, defense, and utility skills",
		L"Tip: Use environment structures for cover and tactical advantages during battles",
		L"Tip: Keep track of monster elemental attacks to adjust your gear and strategies",
		L"Tip: Allocate attribute points wisely to enhance your character's strengths",
		L"Tip: Use area restraints and chokepoints to control enemy movement in combat",
		L"Tip: Save rare scrolls and runes for powerful enchantments and upgrades",
		L"Tip: Customize your skill hotkeys for quick access to essential abilities",
		L"Tip: Experiment with different playstyles to keep your gameplay experience fresh",
		L"Tip: Save high-level challenge areas for moments when you are adequately prepared",
		L"Tip: Use shrines strategically to gain temporary bonuses during critical moments",
		L"Tip: Maintain a stock of basic supplies like potions and scrolls for emergencies",
		L"Tip: Keep track of quest timers to complete time-sensitive objectives",
		L"Tip: Use terrain features to separate enemies and deal with them one by one",
		L"Tip: Save and identify rare charms and amulets for valuable stat bonuses",
		L"Tip: Utilize temporary buffs from shrines and spells for challenging encounters",
		L"Tip: Explore side areas and dungeons for additional loot and experience",
		L"Tip: Learn about enemy resistances to optimize your elemental damage output",
		L"Tip: Use healing abilities and potions strategically to conserve resources",
		L"Tip: Keep an eye out for unique monster variants for rare loot drops",
		L"Tip: Save quest items and keys for their specific usage requirements",
		L"Tip: Use crowd control effects to interrupt enemy spellcasting and attacks",
		L"Tip: Adapt your gear and skills to match the enemy types in different areas",
		L"Tip: Explore different difficulty levels for increased challenges and rewards",
		L"Tip: Save valuable runes and gems for crafting and socketing powerful items",
		L"Tip: Use mercenaries as distractions and additional damage dealers in tough fights",
		L"Tip: Maintain awareness of your character's elemental resistances for survivability",
		L"Tip: Utilize environmental objects for tactical advantages in combat",
		L"Tip: Save high-level runes for crafting end-game gear upgrades",
		L"Tip: Keep track of time-sensitive events and quests for unique rewards",
		L"Tip: Use unique class abilities for strategic advantages in different situations",
		L"Tip: Experiment with synergistic skill combinations for enhanced effectiveness",
		L"Tip: Maintain a diverse potion stock for various combat scenarios",
		L"Tip: Use traps and environmental hazards to your advantage in battles",
		L"Tip: Save rare and unique items for specialized character builds",
		L"Tip: Explore secret passages and hidden areas for valuable treasures",
		L"Tip: Use scrolls and potions strategically to weaken and control enemies",
		L"Tip: Learn about boss mechanics and weaknesses for efficient battles",
		L"Tip: Save powerful consumables for boss encounters and critical moments",
		L"Tip: Explore different skill synergies for powerful character builds",
		L"Tip: Use temporary buffs and enhancements for challenging encounters",
		L"Tip: Maintain a balanced inventory with essential supplies and loot",
		L"Tip: Explore diverse environments for varied monster encounters and rewards",
		L"Tip: Use environmental objects strategically for crowd control",
		L"Tip: Save high-level crafting materials for end-game gear upgrades",
		L"Tip: Prioritize enemy threats based on their damage output and abilities",
		L"Tip: Use area-of-effect skills to handle multiple enemies efficiently",
		L"Tip: Adapt your gear and skills to match enemy resistances and vulnerabilities",
		L"Tip: Maintain a diverse arsenal of weapons for different combat scenarios",
		L"Tip: Explore optional areas and dungeons for valuable loot and experience",
		L"Tip: Use crowd control effects to manage large groups of monsters",
		L"Tip: Save powerful spells and abilities for critical moments and tough enemies",
		L"Tip: Maintain a balanced build between offense, defense, and utility skills",
		L"Tip: Use temporary buffs strategically to overcome challenging encounters",
		L"Tip: Save rare crafting materials for high-tier item upgrades and enchantments",
		L"Tip: Adapt your tactics and strategies to different enemy types and behaviors",
		L"Tip: Explore different elemental damages for varied combat advantages",
		L"Tip: Use defensive skills and abilities to mitigate incoming damage",
		L"Tip: Save powerful potions and consumables for boss battles and emergencies",
		L"Tip: Maintain awareness of your character's status effects and buffs during combat",
		L"Tip: Use hit-and-run tactics to deal with powerful enemies safely",
		L"Tip: Save skill points for later levels to invest in more powerful abilities",
		L"Tip: Utilize in-game lore and clues to uncover hidden treasures and secrets",
		L"Tip: Use environmental obstacles to block enemy paths and control engagements",
		L"Tip: Save rare and unique items for specialized builds and synergies",
		L"Tip: Adapt your playstyle based on the monster density and encounter difficulty",
		L"Tip: Use temporary buffs strategically to enhance your combat effectiveness",
		L"Tip: Save valuable scrolls and runes for crucial moments and upgrades",
		L"Tip: Maintain a balanced approach between offensive and defensive skills",
		L"Tip: Use diverse weapon types to exploit enemy weaknesses and resistances",
		L"Tip: Explore different talent trees to discover unique character synergies",
		L"Tip: Save powerful abilities and cooldowns for boss fights and elite enemies",
		L"Tip: Maintain a varied potion supply for different combat situations",
		L"Tip: Use knowledge of enemy mechanics to plan effective engagement strategies",
		L"Tip: Save high-quality gear for challenging areas and boss encounters",
		L"Tip: Adapt your gear and skills based on the elements prevalent in certain areas",
		L"Tip: Use crowd control effects to manage enemy aggro and group dynamics",
		L"Tip: Save rare and valuable items for trading or future character builds",
		L"Tip: Explore different character builds to find synergies with your preferred playstyle",
		L"Tip: Use terrain features strategically to gain advantages in combat",
		L"Tip: Save powerful consumables for pivotal moments and difficult encounters",
		L"Tip: Maintain a diverse inventory of utility items for various situations",
		L"Tip: Use enemy behaviors and movement patterns to predict their attacks",
		L"Tip: Save rare crafting materials for end-game item enhancements",
		L"Tip: Adapt your strategies and tactics to different boss fight mechanics",
		L"Tip: Use area-of-effect abilities to control and weaken large groups of enemies",
		L"Tip: Save powerful spells and abilities for critical moments and tough enemies",
		L"Tip: Maintain awareness of your character's status effects and debuffs during combat",
		L"Tip: Use hit-and-run tactics to wear down powerful enemies safely",
		L"Tip: Save skill points for later levels to invest in more powerful talents",
		L"Tip: Utilize game lore and quest hints to uncover hidden treasures and secrets",
		L"Tip: Use environmental obstacles and terrain features to control enemy movements",
		L"Tip: Save rare and unique items for specialized character builds and synergies",
		L"Tip: Adapt your playstyle based on the monster types and encounter difficulties",
		L"Tip: Use temporary buffs strategically to enhance your combat capabilities",
		L"Tip: Save valuable scrolls and runes for pivotal moments and item upgrades",
		L"Tip: Maintain a balanced mix of offensive and defensive skills for versatility",
		L"Tip: Use various weapon types to exploit enemy vulnerabilities and resistances",
		L"Tip: Explore different talent paths to uncover powerful character synergies",
		L"Tip: Save powerful abilities and cooldowns for boss encounters and elite fights",
		L"Tip: Maintain a diverse potion supply for handling different combat scenarios",
		L"Tip: Use knowledge of enemy mechanics to plan effective combat strategies",
		L"Tip: Save high-quality gear for challenging encounters and boss battles",
		L"Tip: Adapt your gear and skills according to elemental threats in specific areas",
		L"Tip: Use crowd control effects to manage enemy aggro and group dynamics",
		L"Tip: Save rare and valuable items for trading or future character builds",
		L"Tip: Explore different character builds to find synergies with your preferred style",
		L"Tip: Use terrain features and environmental objects for tactical advantages",
		L"Tip: Save powerful consumables for pivotal moments and challenging fights",
		L"Tip: Maintain a varied inventory of utility items for diverse challenges",
		L"Tip: Use enemy behaviors and patterns to predict and counter their attacks",
		L"Tip: Save rare crafting materials for end-game equipment and enhancements",
		L"Tip: Adapt your tactics and strategies to different boss encounter mechanics",
		L"Tip: Use area-of-effect abilities to control and weaken groups of enemies",
		L"Tip: Save powerful spells and abilities for crucial moments and tough enemies",
		L"Tip: Maintain awareness of your character's status effects during battles",
		L"Tip: Use hit-and-run tactics to handle powerful enemies safely",
		L"Tip: Save skill points for higher levels to invest in powerful talents",
		L"Tip: Utilize game lore and quest details to uncover hidden treasures",
		L"Tip: Use environmental obstacles to control enemy movements during combat",
		L"Tip: Save rare and unique items for specialized builds and synergies",
		L"Tip: Adapt your playstyle to the strengths and weaknesses of different enemies",
		L"Tip: Use temporary buffs strategically for challenging battles and encounters",
		L"Tip: Save valuable scrolls and runes for critical moments and item enhancements",
		L"Tip: Maintain a balanced skill set for versatility in various combat situations",
		L"Tip: Use a variety of weapons to exploit enemy vulnerabilities and resistances",
		L"Tip: Explore different talent paths for unique character playstyles",
		L"Tip: Save powerful abilities for boss fights and elite enemy encounters",
		L"Tip: Maintain a diverse potion supply for handling different combat challenges",
		L"Tip: Use knowledge of enemy mechanics to plan effective combat strategies",
		L"Tip: Save high-quality gear for challenging areas and boss battles",
		L"Tip: Adapt your gear and skills based on the elements prevalent in specific areas",
		L"Tip: Use crowd control effects to manage enemy groups and engagements",
		L"Tip: Save rare and valuable items for trading or future builds",
		L"Tip: Explore different builds to find synergies with your preferred style",
		L"Tip: Use terrain features and environmental objects for tactical advantages",
		L"Tip: Save powerful consumables for crucial moments and difficult fights",
		L"Tip: Maintain a varied inventory of utility items for various challenges",
		L"Tip: Use enemy behaviors and patterns to predict and counter their attacks",
		L"Tip: Save rare crafting materials for end-game equipment and enhancements",
		L"Tip: Adapt your tactics and strategies to different boss mechanics",
		L"Tip: Use area-of-effect abilities to control and weaken large groups of foes",
		L"Tip: Save powerful spells and abilities for critical moments and tough enemies",
		L"Tip: Maintain awareness of your character's status during intense battles",
		L"Tip: Use hit-and-run tactics to handle powerful enemies safely",
		L"Tip: Save skill points for higher levels to invest in powerful talents",
		L"Tip: Utilize game lore and quest details to uncover hidden treasures",
		L"Tip: Use environmental obstacles to control enemy movements during fights",
		L"Tip: Save rare and unique items for specialized builds and synergies",
		L"Tip: Adapt your playstyle to the strengths and weaknesses of different foes",
		L"Tip: Use temporary buffs strategically for challenging encounters",
		L"Tip: Save valuable scrolls and runes for critical moments and enhancements",
		L"Tip: Maintain a balanced skill set for versatility in combat scenarios",
		L"Tip: Use a variety of weapons to exploit enemy vulnerabilities and resistances",
		L"Tip: Explore different talent paths for unique character strategies",
		L"Tip: Save powerful abilities for boss fights and elite engagements",
		L"Tip: Maintain a diverse potion supply for handling different challenges",
		L"Tip: Use knowledge of enemy mechanics to plan effective strategies",
		L"Tip: Save high-quality gear for challenging areas and boss encounters",
		L"Tip: Adapt your gear and skills based on prevalent elements in areas",
		L"Tip: Use crowd control effects to manage enemy groups and dynamics",
		L"Tip: Save rare and valuable items for trading or future builds",
		L"Tip: Explore different builds to find synergies with your preferred style",
		L"Tip: Use terrain features and objects for tactical advantages in battles",
		L"Tip: Save powerful consumables for crucial moments and tough encounters",
		L"Tip: Maintain a varied inventory for handling various challenges",
		L"Tip: Use enemy behaviors and patterns to predict and counter their actions",
		L"Tip: Save rare crafting materials for end-game equipment enhancements",
		L"Tip: Adapt tactics and strategies to different boss fight mechanics",
		L"Tip: Use area-of-effect abilities to control and weaken large groups of enemies",
		L"Tip: Save powerful spells and abilities for critical moments and tough foes",
		L"Tip: Maintain awareness of your character's status during intense battles",
		L"Tip: Use hit-and-run tactics to handle powerful enemies safely",
		L"Tip: Save skill points for higher levels to invest in powerful talents",
		L"Tip: Utilize game lore and quest details to uncover hidden treasures",
		L"Tip: Use environmental obstacles to control enemy movements during fights",
		L"Tip: Save rare and unique items for specialized builds and synergies",
		L"Tip: Adapt your playstyle to the strengths and weaknesses of different foes",
		L"Tip: Use temporary buffs strategically for challenging encounters",
		L"Tip: Save valuable scrolls and runes for critical moments and enhancements",
		L"Tip: Maintain a balanced skill set for versatility in combat scenarios",
		L"Tip: Use a variety of weapons to exploit enemy vulnerabilities and resistances",
		L"Tip: Explore different talent paths for unique character strategies",
		L"Tip: Save powerful abilities for boss fights and elite engagements",
		L"Tip: Maintain a diverse potion supply for handling different challenges",
		L"Tip: Use knowledge of enemy mechanics to plan effective strategies",
		L"Tip: Save high-quality gear for challenging areas and boss encounters",
		L"Tip: Adapt your gear and skills based on prevalent elements in areas",
		L"Tip: Use crowd control effects to manage enemy groups and dynamics",
		L"Tip: Save rare and valuable items for trading or future builds",
		L"Tip: Explore different builds to find synergies with your preferred style",
		L"Tip: Use terrain features and objects for tactical advantages in battles",
		L"Tip: Save powerful consumables for crucial moments and tough encounters",
		L"Tip: Maintain a varied inventory for handling various challenges",
		L"Tip: Use enemy behaviors and patterns to predict and counter their actions",
		L"Tip: Save rare crafting materials for end-game equipment enhancements",
		L"Tip: Adapt tactics and strategies to different boss fight mechanics",
		L"Tip: Use area-of-effect abilities to control and weaken large groups of enemies",
		L"Tip: Save powerful spells and abilities for critical moments and tough foes",
		L"Tip: Maintain awareness of your character's status during intense battles",
		L"Tip: Use hit-and-run tactics to handle powerful enemies safely",
		L"Tip: Save skill points for higher levels to invest in powerful talents",
		L"Tip: Utilize game lore and quest details to uncover hidden treasures",
		L"Tip: Use environmental obstacles to control enemy movements during fights",
		L"Tip: Save rare and unique items for specialized builds and synergies",
		L"Tip: Adapt your playstyle to the strengths and weaknesses of different foes",
		L"Tip: Use temporary buffs strategically for challenging encounters",
		L"Tip: Save valuable scrolls and runes for critical moments and enhancements",
		L"Tip: Maintain a balanced skill set for versatility in combat scenarios",
		L"Tip: Use a variety of weapons to exploit enemy vulnerabilities and resistances",
		L"Tip: Explore different talent paths for unique character strategies",
		L"Tip: Save powerful abilities for boss fights and elite engagements",
		L"Tip: Maintain a diverse potion supply for handling different challenges",
		L"Tip: Use knowledge of enemy mechanics to plan effective strategies",
		L"Tip: Save high-quality gear for challenging areas and boss encounters",
		L"Tip: Adapt your gear and skills based on prevalent elements in areas",
		L"Tip: Use crowd control effects to manage enemy groups and dynamics",
		L"Tip: Save rare and valuable items for trading or future builds",
		L"Tip: Explore different builds to find synergies with your preferred style",
		L"Tip: Use terrain features and objects for tactical advantages in battles",
		L"Tip: Save powerful consumables for crucial moments and tough encounters",
		L"Tip: Maintain a varied inventory for handling various challenges",
		L"Tip: Use enemy behaviors and patterns to predict and counter their actions",
		L"Tip: Save rare crafting materials for end-game equipment enhancements",
		L"Tip: Adapt tactics and strategies to different boss fight mechanics",
		L"Tip: Use area-of-effect abilities to control and weaken large groups of enemies",
		L"Tip: Save powerful spells and abilities for critical moments and tough foes",
		L"Tip: Maintain awareness of your character's status during intense battles",
		L"Tip: Use hit-and-run tactics to handle powerful enemies safely",
		L"Tip: Save skill points for higher levels to invest in powerful talents",
		L"Tip: Utilize game lore and quest details to uncover hidden treasures",
		L"Tip: Use environmental obstacles to control enemy movements during fights",
		L"Tip: Save rare and unique items for specialized builds and synergies",
		L"Tip: Adapt your playstyle to the strengths and weaknesses of different foes",
		L"Tip: Use temporary buffs strategically for challenging encounters",
		L"Tip: Save valuable scrolls and runes for critical moments and enhancements",
		L"Tip: Maintain a balanced skill set for versatility in combat scenarios",
		L"Tip: Use a variety of weapons to exploit enemy vulnerabilities and resistances",
		L"Tip: Explore different talent paths for unique character strategies",
		L"Tip: Save powerful abilities for boss fights and elite engagements",
		L"Tip: Maintain a diverse potion supply for handling different challenges",
		L"Tip: Use knowledge of enemy mechanics to plan effective strategies",
		L"Tip: Save high-quality gear for challenging areas and boss encounters",
		L"Tip: Adapt your gear and skills based on prevalent elements in areas",
		L"Tip: Use crowd control effects to manage enemy groups and dynamics",
		L"Tip: Save rare and valuable items for trading or future builds",
		L"Tip: Explore different builds to find synergies with your preferred style",
		L"Tip: Use terrain features and objects for tactical advantages in battles",
		L"Tip: Save powerful consumables for crucial moments and tough encounters",
		L"Tip: Maintain a varied inventory for handling various challenges",
		L"Tip: Use enemy behaviors and patterns to predict and counter their actions",
		L"Tip: Save rare crafting materials for end-game equipment enhancements",
		L"Tip: Adapt tactics and strategies to different boss fight mechanics",
		L"Tip: Use area-of-effect abilities to control and weaken large groups of enemies",
		L"Tip: Save powerful spells and abilities for critical moments and tough foes",
		L"Tip: Maintain awareness of your character's status during intense battles",
		L"Tip: Use hit-and-run tactics to handle powerful enemies safely",
		L"Tip: Save skill points for higher levels to invest in powerful talents",
		L"Tip: Utilize game lore and quest details to uncover hidden treasures",
		L"Tip: Use environmental obstacles to control enemy movements during fights",
		L"Tip: Save rare and unique items for specialized builds and synergies",
		L"Tip: Adapt your playstyle to the strengths and weaknesses of different foes",
		L"Tip: Use temporary buffs strategically for challenging encounters",
		L"Tip: Save valuable scrolls and runes for critical moments and enhancements",
		L"Tip: Maintain a balanced skill set for versatility in combat scenarios",
		L"Tip: Use a variety of weapons to exploit enemy vulnerabilities and resistances",
		L"Tip: Explore different talent paths for unique character strategies",
		L"Tip: Save powerful abilities for boss fights and elite engagements",
		L"Tip: Maintain a diverse potion supply for handling different challenges",
		L"Tip: Use knowledge of enemy mechanics to plan effective strategies",
		L"Tip: Save high-quality gear for challenging areas and boss encounters",
		L"Tip: Adapt your gear and skills based on prevalent elements in areas",
		L"Tip: Use crowd control effects to manage enemy groups and dynamics",
		L"Tip: Save rare and valuable items for trading or future builds",
		L"Tip: Explore different builds to find synergies with your preferred style",
		L"Tip: Use terrain features and objects for tactical advantages in battles",
		L"Tip: Save powerful consumables for crucial moments and tough encounters",
		L"Tip: Maintain a varied inventory for handling various challenges",
		L"Tip: Use enemy behaviors and patterns to predict and counter their actions",
		L"Tip: Save rare crafting materials for end-game equipment enhancements",
		L"Tip: Adapt tactics and strategies to different boss fight mechanics",
		L"Tip: Use area-of-effect abilities to control and weaken large groups of enemies",
		L"Tip: Save powerful spells and abilities for critical moments and tough foes",
		L"Tip: Maintain awareness of your character's status during intense battles",
		L"Tip: Use hit-and-run tactics to handle powerful enemies safely",
		L"Tip: Save skill points for higher levels to invest in powerful talents",
		L"Tip: Utilize game lore and quest details to uncover hidden treasures",
		L"Tip: Use environmental obstacles to control enemy movements during fights",
		L"Tip: Save rare and unique items for specialized builds and synergies",
		L"Tip: Adapt your playstyle to the strengths and weaknesses of different foes",
		L"Tip: Use temporary buffs strategically for challenging encounters",
		L"Tip: Save valuable scrolls and runes for critical moments and enhancements",
		L"Tip: Maintain a balanced skill set for versatility in combat scenarios",
		L"Tip: Use a variety of weapons to exploit enemy vulnerabilities and resistances",
		L"Tip: Explore different talent paths for unique character strategies",
		L"Tip: Save powerful abilities for boss fights and elite engagements",
		L"Tip: Maintain a diverse potion supply for handling different challenges",
		L"Tip: Use knowledge of enemy mechanics to plan effective strategies",
		L"Tip: Save high-quality gear for challenging areas and boss encounters",
		L"Tip: Adapt your gear and skills based on prevalent elements in areas",
		L"Tip: Use crowd control effects to manage enemy groups and dynamics",
		L"Tip: Save rare and valuable items for trading or future builds",
		L"Tip: Explore different builds to find synergies with your preferred style",
		L"Tip: Use terrain features and objects for tactical advantages in battles",
		L"Tip: Save powerful consumables for crucial moments and tough encounters",
		L"Tip: Maintain a varied inventory for handling various challenges",
		L"Tip: Use enemy behaviors and patterns to predict and counter their actions",
		L"Tip: Save rare crafting materials for end-game equipment enhancements",
		L"Tip: Adapt tactics and strategies to different boss fight mechanics",
		L"Tip: Use area-of-effect abilities to control and weaken large groups of enemies",
		L"Tip: Save powerful spells and abilities for critical moments and tough foes",
		L"Tip: Maintain awareness of your character's status during intense battles",
		L"Tip: Use hit-and-run tactics to handle powerful enemies safely",
		L"Tip: Save skill points for higher levels to invest in powerful talents",
		L"Tip: Utilize game lore and quest details to uncover hidden treasures",
		L"Tip: Use environmental obstacles to control enemy movements during fights",
		L"Tip: Save rare and unique items for specialized builds and synergies",
		L"Tip: Adapt your playstyle to the strengths and weaknesses of different foes",
		L"Tip: Use temporary buffs strategically for challenging encounters",
		L"Tip: Save valuable scrolls and runes for critical moments and enhancements",
		L"Tip: Maintain a balanced skill set for versatility in combat scenarios",
		L"Tip: Use a variety of weapons to exploit enemy vulnerabilities and resistances",
		L"Tip: Explore different talent paths for unique character strategies",
		L"Tip: Save powerful abilities for boss fights and elite engagements",
		L"Tip: Maintain a diverse potion supply for handling different challenges",
		L"Tip: Use knowledge of enemy mechanics to plan effective strategies",
		L"Tip: Save high-quality gear for challenging areas and boss encounters",
		L"Tip: Adapt your gear and skills based on prevalent elements in areas",
		L"Tip: Use crowd control effects to manage enemy groups and dynamics",
		L"Tip: Save rare and valuable items for trading or future builds",
		L"Tip: Explore different builds to find synergies with your preferred style",
		L"Tip: Use terrain features and objects for tactical advantages in battles",
		L"Tip: Save powerful consumables for crucial moments and tough encounters",
		L"Tip: Maintain a varied inventory for handling various challenges",
		L"Tip: Use enemy behaviors and patterns to predict and counter their actions",
		L"Tip: Save rare crafting materials for end-game equipment enhancements",
		L"Tip: Adapt tactics and strategies to different boss fight mechanics",
		L"Tip: Use area-of-effect abilities to control and weaken large groups of enemies",
		L"Tip: Save powerful spells and abilities for critical moments and tough foes",
		L"Tip: Maintain awareness of your character's status during intense battles",
		L"Tip: Use hit-and-run tactics to handle powerful enemies safely",
		L"Tip: Save skill points for higher levels to invest in powerful talents",
		L"Tip: Utilize game lore and quest details to uncover hidden treasures",
		L"Tip: Use environmental obstacles to control enemy movements during fights",
		L"Tip: Save rare and unique items for specialized builds and synergies",
		L"Tip: Adapt your playstyle to the strengths and weaknesses of different foes",
		L"Tip: Use temporary buffs strategically for challenging encounters",
		L"Tip: Save valuable scrolls and runes for critical moments and enhancements",
		L"Tip: Maintain a balanced skill set for versatility in combat scenarios",
		L"Tip: Use a variety of weapons to exploit enemy vulnerabilities and resistances",
		L"Consume spirits like potions to increase Spirits Quaffed stat in character stat sheet page",
		L"Cube Souls with Energy Sphere to capture Soul Energy and use it to create powerful items. Recipes will come later",
		L"Collect Demon souls from demons and regular souls from monsters; Demon souls morph you into the demon whose soul you have",
		L"Spirits can be consumed like potions or put on as rings to add to Souls Consumed count",
		L"Cube underused magic/rare/set/unique items with Energy Sphere to collect their magic energy for creating new items",
		L"Cube white items with Energy Sphere to break down into simple item parts for creating base item types with smithing hammers",
		L"Use Rare Shard, Set Stone, or Unique Particle with smithing hammers and Energy Sphere to create rare/set/unique items",
		L"Acquire smithing hammers to create base item types, each requiring X number of collected item parts",
		L"Capture Talisman of Corruption and cube to corrupt items for new magical properties; a gamble with potential rewards",
		L"Use Experience Book for extra experience, Infinite Mana Elixir for massive mana regeneration, and Regeneration Elixir for life regeneration",
		L"Use Elixirs to permanently boost stats like Strength, Dexterity, Health, Stamina, Mana, and resistances",
		L"Equip Sling Stone for increased throwing potion damage, various elemental stones for resistance boosts, and Mastery Stones for skill damage boosts",
		L"Use Portable Shrines for temporary buffs like increased defense, damage, experience gained, mana regeneration, and skill levels",
		L"In Ironman, gold is nonexistent, town portals are disabled; rely on items, waypoints, and strategic gameplay to progress",
		L"Inventory management is critical; use items conservatively and intelligently switch gear based on monster types and situations",
		L"Hybrid character builds and creative weapon choices are encouraged in Ironman; experiment and adapt strategies accordingly",
		L"Protect hirelings actively as they are essential allies; they can be hired for 0 gold but lose items if they die",
		L"Coordinate with teammates in MultiPlayer; teamwork, resource sharing, and strategy are key to success in Ironman",
		L"In Hardcore Ironman, caution and careful planning are crucial; falling back, potion management, and adaptability are essential for survival",
		L"Utilize waypoints for inter-level travel in Ironman; waypoints become crucial for navigating the game world",
		L"Always have backup gear in your backpack in Ironman; switch out gear intelligently based on monster types and challenges",
		L"Experiment with different weapons and gear in Ironman; adapt your strategies to overcome various monster types and situations",
		L"Keep a close watch on your hirelings' health in Ironman; losing a hireling means losing their equipped items as well",
		L"Plan and communicate effectively in MultiPlayer Ironman games; teamwork and coordination are key to overcoming challenges",
		L"Be conservative with resources in Hardcore Ironman; strategic planning and careful execution are vital for survival",
		L"Craft rare/set/unique items using collected item parts and smithing hammers in Ironman; choose item types wisely for your build",
		L"Capture and corrupt items using Talisman of Corruption for new magical properties in Ironman; take calculated risks for potential rewards",
		L"Use Portable Shrines strategically in Ironman for temporary buffs; timing and placement of shrines can turn the tide of battle",
		L"Keep track of Souls Consumed and Spirits Quaffed stats in Ironman; they reflect your progress and resource utilization in the game",
		L"Enhance your character with Elixirs and Stones for permanent stat boosts and bonuses in Ironman; plan your upgrades wisely",
		L"Master the art of retreating and regrouping in Hardcore Ironman; strategic fallbacks can save your character's life in tough situations",
		L"Stay vigilant and adapt your strategies in MultiPlayer Ironman games; synergy and cooperation with teammates are essential for success",
		L"Experiment with different skill synergies and builds in Ironman; explore the full potential of your character's abilities",
		L"Craft and equip powerful unique items using collected Magic Essence Points and recipes in Ironman; customize your gear for optimal performance",
		L"Explore new areas and challenges in Ironman; discover hidden secrets and encounters by venturing off the beaten path",
		L"Combine different magic/rare/set/unique items with the Energy Sphere to collect their magic energy in Ironman",
		L"Cube white items with the Energy Sphere to break them down into simple item parts for crafting in Ironman",
		L"Use smithing hammers with the Energy Sphere to create base item types for crafting in Ironman",
		L"Combine Rare Shard, Set Stone, or Unique Particle with base item types and Magic Essence Points to create rare/set/unique items in Ironman",
		L"Cube Souls with the Energy Sphere to capture their energy and use it for creating more powerful items in Ironman",
		L"Be careful out there!",
		L"+ to Amazon Skills increases the Minimum and Maximum skill levels for Amazon skills",
		L"+ to Assassin Skills increases the Minimum and Maximum skill levels for Assassin skills",
		L"+ to Barbarian Skills increases the Minimum and Maximum skill levels for Barbarian skills",
		L"+ to Druid Skills increases the Minimum and Maximum skill levels for Druid skills",
		L"+ to Necromancer Skills increases the Minimum and Maximum skill levels for Necromancer skills",
		L"+ to Paladin Skills increases the Minimum and Maximum skill levels for Paladin skills",
		L"+ to Sorceress Skills increases the Minimum and Maximum skill levels for Sorceress skills",
		L"+ to All Skills increases the Minimum and Maximum skill levels for all skills",
		L"+ to Fire Skills increases the Minimum and Maximum skill levels for Fire skills",
		L"+ to Cold Skills increases the Minimum and Maximum skill levels for Cold skills",
		L"+ to Lightning Skills increases the Minimum and Maximum skill levels for Lightning skills",
		L"+ to Poison Skills increases the Minimum and Maximum skill levels for Poison skills",
		L"+ to Magic Skills increases the Minimum and Maximum skill levels for Magic skills",
		L"+ to Single Skills increases the Minimum and Maximum levels for a specific skill",
		L"+ to Single Skill Tabs increases the Minimum and Maximum levels for a specific skill tab",
		L"Chance to cast a skill when attacking triggers a specified skill with a certain chance and skill level",
		L"Chance to cast a skill when hitting triggers a specified skill with a certain chance and skill level",
		L"Chance to cast a skill when getting hit triggers a specified skill with a certain chance and skill level",
		L"Skill Charges grant a certain number of charges to a specific skill with a designated skill level",
		L"Using an Aura when equipped activates a specified aura skill with Minimum and Maximum levels",
		L"Cast a skill when a monster is killed has a chance to trigger a specified skill at a certain skill level",
		L"Cast a skill when a player is killed has a chance to trigger a specified skill at a certain skill level",
		L"Cast a skill when a player level-up has a chance to trigger a specified skill at a certain skill level",
		L"Spawns a random skill within a range of specified skills with corresponding skill levels",
		L"+ to other char classes grants skills from other character classes with Minimum and Maximum levels",
		L"Tip: Keep your mercenary well-equipped with gear that complements your own strengths and weaknesses",
		L"+ to AC increases the Armor Class within a specified range",
		L"+ X to AC per level increases Armor Class by a certain value per character level",
		L"+ X to AC Based on Strength increases Armor Class based on Strength by a certain value",
		L"+ X to AC Based on Dexterity increases Armor Class based on Dexterity by a certain value",
		L"+% to AC increases Armor Class by a percentage within a specified range",
		L"+% X to AC per level increases Armor Class by a certain percentage per character level",
		L"+% X to AC Based on Strength increases Armor Class based on Strength by a certain percentage",
		L"+% X to AC Based on Dexterity increases Armor Class based on Dexterity by a certain percentage",
		L"+ to AC vs Melee increases Armor Class against melee attacks within a specified range",
		L"+ to AC vs Missiles increases Armor Class against missile attacks within a specified range",
		L"Tip: Pay attention to enemy resistances and weaknesses to effectively choose your elemental attacks",
		L"+ to Durability increases the durability of an item within a specified range",
		L"+% to Durability increases the durability of an item by a percentage within a specified range",
		L"Indestructible makes an item not lose durability",
		L"Repair 1 Dur per 100/X seconds repairs an item's durability over time",
		L"Repair 1 Qnt per 100/X seconds repairs an item's quantity over time",
		L"Increase Stack by X amount increases the stack size of an item by a certain value",
		L"Tip: Identify unidentified items at the shop to reveal their true potential before selling or using them",
		L"+ to AR increases Attack Rating within a specified range",
		L"+ Attack Rating per level increases Attack Rating by a certain value per character level",
		L"+ Attack Rating based on Strength increases Attack Rating based on Strength by a certain value",
		L"+ Attack Rating based on Dexterity increases Attack Rating based on Dexterity by a certain value",
		L"+% to AR increases Attack Rating by a percentage within a specified range",
		L"+% AR per level increases Attack Rating by a certain percentage per character level",
		L"+% AR based on Strength increases Attack Rating based on Strength by a certain percentage",
		L"+% AR based on Dexterity increases Attack Rating based on Dexterity by a certain percentage",
		L"+ to AR vs Demons increases Attack Rating against Demons within a specified range",
		L"+ to AR vs Undead increases Attack Rating against Undead within a specified range",
		L"+% to AR vs Montype increases Attack Rating against a specified monster type within a specified range",
		L"Tip: Take advantage of town portals to quickly return to town for repairs and restocking",
		L"+ to Damage increases both Minimum and Maximum damage values for an item within a specified range",
		L"+% to Damage increases both Minimum and Maximum damage values by a percentage within a specified range",
		L"+ to Minimum Damage increases Minimum damage within a specified range",
		L"+ to Minimum Damage per Level increases Minimum damage by a certain value per character level",
		L"+ to Minimum Damage based on Strength increases Minimum damage based on Strength by a certain value",
		L"+ to Minimum Damage based on Dexterity increases Minimum damage based on Dexterity by a certain value",
		L"+% to Minimum Damage per level increases Minimum damage by a percentage per character level",
		L"+ to Maximum Damage increases Maximum damage within a specified range",
		L"+ to Maximum Damage per level increases Maximum damage by a certain value per character level",
		L"+ to Maximum Damage based on Strength increases Maximum damage based on Strength by a certain value",
		L"+ to Maximum Damage based on Dexterity increases Maximum damage based on Dexterity by a certain value",
		L"+% to Maximum damage per level increases Maximum damage by a percentage per character level",
		L"+% to Maximum Damage based on Strength increases Maximum damage based on Strength by a certain percentage",
		L"+% to Maximum Damage based on Dexterity increases Maximum damage based on Dexterity by a certain percentage",
		L"+ to Minimum & Maximum Damage increases both Minimum and Maximum damage values for an item within a specified range",
		L"+% to Damage vs Demons increases damage against Demons within a specified range",
		L"+% damage to demons per level increases damage against Demons by a certain percentage per character level",
		L"+% to Damage vs Undead increases damage against Undead within a specified range",
		L"+% damage to undead per level increases damage against Undead by a certain percentage per character level",
		L"+% to crushing blow increases the chance to cause a crushing blow within a specified range",
		L"+% crushing blow per level increases the chance to cause a crushing blow by a certain percentage per character level",
		L"+% crushing blow based on Strength increases the chance to cause a crushing blow based on Strength by a certain percentage",
		L"+% crushing blow based on Dexterity increases the chance to cause a crushing blow based on Dexterity by a certain percentage",
		L"+% to deadly strike increases the chance to cause a deadly strike within a specified range",
		L"+% deadly strike per level increases the chance to cause a deadly strike by a certain percentage per character level",
		L"+% deadly strike based on Strength increases the chance to cause a deadly strike based on Strength by a certain percentage",
		L"+% deadly strike Based on Dexterity increases the chance to cause a deadly strike based on Dexterity by a certain percentage",
		L"+ to throw damage increases throwing damage within a specified range",
		L"+% to Damage vs Montype increases damage against a specified monster type within a specified range",
		L"Tip: Always carry a few antidote potions to counter poison attacks from enemies",
		L"Ignores Target Defense (ITD) ignores the target's defense",
		L"+% pierce increases the chance to pierce enemy resistances within a specified range",
		L"+% pierce based on Level increases the chance to pierce enemy resistances based on character level",
		L"+% pierce based on Strength increases the chance to pierce enemy resistances based on Strength",
		L"Knockback has a chance to knockback enemies on hit",
		L"+% life stolen grants life steal within a specified range",
		L"+% mana stolen grants mana steal within a specified range",
		L"+# mana after each kill grants mana regeneration after each kill within a specified range",
		L"+# after each demon kill grants health after each demon kill within a specified range",
		L"Prevents monster healing prevents monsters from healing",
		L"+% chance to cause open wounds increases the chance to cause open wounds within a specified range",
		L"+% open wounds per level increases the chance to cause open wounds based on character level",
		L"+% open wounds based on Strength increases the chance to cause open wounds based on Strength",
		L"+% open wounds Based on Dexterity increases the chance to cause open wounds based on Dexterity",
		L"Enemy Is slowed by X% slows enemies by a percentage within a specified range",
		L"Enemy Is slowed by X% based on Level slows enemies based on character level",
		L"Chance to Blind Target has a chance to blind the target",
		L"Chance to Blind Target based on Level has a chance to blind the target based on character level",
		L"Freeze Target X seconds freezes the target for a specified duration",
		L"Freeze Target X seconds based on Level freezes the target based on character level",
		L"Freeze Target X seconds based on Energy freezes the target based on character Energy",
		L"Reduce AC by X% reduces the target's Armor Class by a percentage within a specified range",
		L"Reduce AC by X reduces the target's Armor Class within a specified range",
		L"+ to Kicking Damage increases kicking damage within a specified range",
		L"+ Kick damage per level increases kicking damage based on character level",
		L"+ Kick damage based on Strength increases kicking damage based on Strength",
		L"+ Kick damage based on Dexterity increases kicking damage based on Dexterity",
		L"+% chance to reanimate target increases the chance to reanimate a target within a specified range",
		L"Reduces monster fire resistance reduces a monster's fire resistance within a specified range",
		L"Reduces monster lightning resistance reduces a monster's lightning resistance within a specified range",
		L"Reduces monster cold resistance reduces a monster's cold resistance within a specified range",
		L"Reduces monster poison resistance reduces a monster's poison resistance within a specified range",
		L"HP gained after every kill grants health regeneration after each kill within a specified range",
		L"Corspe cannot be ressurected/Targeted prevents corpses from being resurrected or targeted",
		L"Tip: Experiment with different skill combinations to find the most effective build for your playstyle",
		L"+% damage taken goes to mana converts a percentage of damage taken to mana within a specified range",
		L"Damage reduced by X amount reduces incoming damage by a specific amount within a specified range",
		L"Damage reduced by X% reduces incoming damage by a percentage within a specified range",
		L"Damage reduced by X% based on Level reduces incoming damage based on character level",
		L"Damage reduced by X% based on Vitality reduces incoming damage based on character Vitality",
		L"Magic damage reduced by X reduces incoming magic damage within a specified range",
		L"Magic damage reduced by X per Level reduces incoming magic damage based on character level",
		L"Magic damage reduced by X per Energy reduces incoming magic damage based on character Energy",
		L"Attacker takes X damage when hitting causes an attacker to take damage when hitting",
		L"Attacker takes X damage per level causes an attacker to take damage based on character level",
		L"Attacker takes X ltng dmg when hitting causes an attacker to take lightning damage when hitting",
		L"Attacker takes X fire dmg when hitting causes an attacker to take fire damage when hitting",
		L"Attacker takes X cold dmg when hitting causes an attacker to take cold damage when hitting",
		L"Attacker X chance to flee when hitting gives the attacker a chance to flee when hitting",
		L"Attacker X chance get blinded when hitting gives the attacker a chance to get blinded when hitting",
		L"Tip: Keep an eye on your stamina bar during long journeys to avoid becoming exhausted",
		L"Cold Damage adds cold damage to attacks or skills over a specified duration",
		L"Minimum Cold Damage sets the minimum cold damage for attacks or skills",
		L"Maximum Cold Damage sets the maximum cold damage for attacks or skills",
		L"Cold Duration sets the duration of cold effects caused by attacks or skills",
		L"Poison Damage adds poison damage to attacks or skills over a specified duration",
		L"Minimum Poison Damage sets the minimum poison damage for attacks or skills",
		L"Maximum Poison Damage sets the maximum poison damage for attacks or skills",
		L"Poison Duration sets the duration of poison effects caused by attacks or skills",
		L"Damage per Poison Level increases poison damage per character level",
		L"Poison Length Reduction reduces the duration of poison effects caused by attacks or skills",
		L"Poison Length Reduction per level reduces the duration of poison effects based on character level",
		L"Poison Length Reduction per Vitality reduces the duration of poison effects based on character Vitality",
		L"Fire Damage adds fire damage to attacks or skills over a specified duration",
		L"Minimum Fire Damage sets the minimum fire damage for attacks or skills",
		L"Maximum Fire Damage sets the maximum fire damage for attacks or skills",
		L"Fire Duration sets the duration of fire effects caused by attacks or skills",
		L"Fire Length Reduction reduces the duration of fire effects caused by attacks or skills",
		L"Fire Length Reduction per level reduces the duration of fire effects based on character level",
		L"Fire Length Reduction per Vitality reduces the duration of fire effects based on character Vitality",
		L"Lightning Damage adds lightning damage to attacks or skills over a specified duration",
		L"Minimum Lightning Damage sets the minimum lightning damage for attacks or skills",
		L"Maximum Lightning Damage sets the maximum lightning damage for attacks or skills",
		L"Lightning Duration sets the duration of lightning effects caused by attacks or skills",
		L"Magic Damage adds magic damage to attacks or skills over a specified duration",
		L"Minimum Magic Damage sets the minimum magic damage for attacks or skills",
		L"Maximum Magic Damage sets the maximum magic damage for attacks or skills",
		L"Magic Duration sets the duration of magic effects caused by attacks or skills",
		L"Damage per Magic Level increases magic damage per character level",
		L"Magic Resistance Reduction reduces enemy resistance against magic attacks",
		L"Monster Flee X% causes monsters to flee with a certain chance",
		L"Chance to create an explosion creates an explosion with a certain chance",
		L"Fire explosive arrows causes arrows to explode on impact with a certain chance",
		L"Half Freeze Duration halves the duration of freeze effects",
		L"Cannot be Frozen prevents the character from being frozen",
		L"+% to Resist All increases resistance to all damage types within a specified range",
		L"+% to Resist All based on Level increases resistance to all damage types based on character level",
		L"+% to Resist Cold increases resistance to cold damage within a specified range",
		L"+% to Resist Cold per level increases resistance to cold damage based on character level",
		L"+% to Resist Fire increases resistance to fire damage within a specified range",
		L"+% to Resist Fire per level increases resistance to fire damage based on character level",
		L"+% to Resist Lightning increases resistance to lightning damage within a specified range",
		L"+% to Resist Lightning per level increases resistance to lightning damage based on character level",
		L"+% to Resist Poison increases resistance to poison damage within a specified range",
		L"+% to Resist Poison per level increases resistance to poison damage based on character level",
		L"+% to Resist Magic increases resistance to magic damage within a specified range",
		L"+% to Resist Magic based on Level increases resistance to magic damage based on character level",
		L"+% to Resist Magic based on Energy increases resistance to magic damage based on character Energy",
		L"+% to Maximum Resist All increases maximum resistance to all damage types within a specified range",
		L"+% to Maximum Resist Cold increases maximum resistance to cold damage within a specified range",
		L"+% to Maximum Resist Fire increases maximum resistance to fire damage within a specified range",
		L"+% to Maximum Resist Lightning increases maximum resistance to lightning damage within a specified range",
		L"+% to Maximum Resist Poison increases maximum resistance to poison damage within a specified range",
		L"+% to Maximum Resist Magic increases maximum resistance to magic damage within a specified range",
		L"Reduce Poison Duration reduces the duration of poison effects on the character",
		L"Reduce Poison Duration per Level reduces the duration of poison effects based on character level",
		L"Reduce Poison Duration per Vitality reduces the duration of poison effects based on character Vitality",
		L"Reduce Curse Duration reduces the duration of curse effects on the character",
		L"Reduce Curse Duration per Level reduces the duration of curse effects based on character level",
		L"Reduce Curse Duration per Energy reduces the duration of curse effects based on character Energy",
		L"Tip: Utilize the Horadric Cube to combine items for unique bonuses and effects",
		L"Presence of 'res-all-max-hidden', 'res-all-hidden', 'res-all-max%', 'all-zero-display' on the same item displays and modifies all resistances",
		L"+ to Dexterity increases Dexterity within a specified range",
		L"+ to Dexterity per level increases Dexterity by a certain value per character level",
		L"+% to Dexterity increases Dexterity by a percentage within a specified range",
		L"+% to Dexterity per level increases Dexterity by a percentage per character level",
		L"+ to Strength increases Strength within a specified range",
		L"+ to Strength per level increases Strength by a certain value per character level",
		L"+% to Strength increases Strength by a percentage within a specified range",
		L"+% to Strength per level increases Strength by a percentage per character level",
		L"+ to Vitality increases Vitality within a specified range",
		L"+ to Vitality per level increases Vitality by a certain value per character level",
		L"+% to Vitality increases Vitality by a percentage within a specified range",
		L"+% to Vitality per level increases Vitality by a percentage per character level",
		L"+ to Energy increases Energy within a specified range",
		L"+ to Energy per level increases Energy by a certain value per character level",
		L"+% to Energy increases Energy by a percentage within a specified range",
		L"+% to Energy per level increases Energy by a percentage per character level",
		L"Adds + to All stats increases all stats within a specified range",
		L"+ to Health increases Health within a specified range",
		L"+ to Health per level increases Health by a certain value per character level",
		L"+ to Health based on Vitality increases Health based on Vitality by a certain value",
		L"+% to Health increases Health by a percentage within a specified range",
		L"+ to Mana increases Mana within a specified range",
		L"+ to Mana per level increases Mana by a certain value per character level",
		L"+ to Mana based on Energy increases Mana based on Energy by a certain value",
		L"+% to Mana increases Mana by a percentage within a specified range",
		L"+ to Replenish Life grants life replenishment within a specified range",
		L"+% to Mana Regeneration increases Mana regeneration by a percentage within a specified range",
		L"+% run/walk speed increases running and walking speed within a specified range",
		L"+% run/walk speed based on Level increases running and walking speed based on character level",
		L"+% run/walk speed based on Vitality increases running and walking speed based on Vitality",
		L"+ to stamina increases Stamina within a specified range",
		L"+ stamina per level increases Stamina by a certain value per character level",
		L"+% reduce stamina drain decreases the rate at which stamina is drained within a specified range",
		L"+% stamina regeneration increases stamina regeneration rate within a specified range",
		L"+% regenerate stamina per level increases stamina regeneration rate based on character level",
		L"+% attack speed increases attack speed within a specified range",
		L"+% attack speed based on Level increases attack speed based on character level",
		L"+% attack speed based on Strength increases attack speed based on Strength",
		L"+% attack speed based on Dexterity increases attack speed based on Dexterity",
		L"+% chance to block increases the chance to block within a specified range",
		L"+% chance to block based on Level increases the chance to block based on character level",
		L"+% chance to block based on Strength increases the chance to block based on Strength",
		L"+% chance to block based on Dexterity increases the chance to block based on Dexterity",
		L"+% block speed increases block speed within a specified range",
		L"+% block speed based on Level increases block speed based on character level",
		L"+% block speed based on Strength increases block speed based on Strength",
		L"+% block speed based on Dexterity increases block speed based on Dexterity",
		L"+% cast speed increases casting speed within a specified range",
		L"+% cast speed based on Level increases casting speed based on character level",
		L"+% cast speed based on Energy increases casting speed based on character Energy",
		L"+% hit recovery speed increases hit recovery speed within a specified range",
		L"+% hit recovery speed based on Level increases hit recovery speed based on character level",
		L"+% hit recovery speed based on Vitality increases hit recovery speed based on Vitality",
		L"+ to Light Radius increases the light radius within a specified range",
		L"Fire Magic Arrows enables firing magical arrows",
		L"Fire Explosive Arrows enables firing explosive arrows",
		L"+% chance to find magic items increases the chance to find magic items within a specified range",
		L"+ Magic Find per level increases the chance to find magic items based on character level",
		L"+% increased gold drops increases the amount of gold dropped by monsters within a specified range",
		L"+ More Gold per Level increases the amount of gold dropped by monsters based on character level",
		L"Altered Item Requirements reduces the requirements to equip an item within a specified range",
		L"Chance to make enemies flee increases the chance of causing enemies to flee within a specified range",
		L"Add X Sockets to an Item adds a specific number of sockets to an item",
		L"Makes any item throwable enables throwing any item",
		L"Adds additional blood increases the amount of blood displayed when hitting enemies",
		L"+% additional xp gained increases the amount of experience gained within a specified range",
		L"+ additional xp gained per level increases the amount of experience gained based on character level",
		L"+ additional xp gained based on Energy increases the amount of experience gained based on character Energy",
		L"Reduce vendor cost reduces the cost of items bought from vendors within a specified range",
		L"Reduce vendor cost based on Level reduces the cost of items based on character level",
		L"Applies fade state changes the character to a faded state within a specified range",
		L"Adds additional level requirements increases the level requirement to use an item",
		L"Force spawns item as ethereal makes an item always spawn as an ethereal item",
		L"Tip: Explore all corners of the map to uncover hidden treasures and secret passages",
		L"Tip: Keep an eye on your surroundings for environmental hazards like traps and pitfalls",
		L"Tip: Experiment with different character builds and skill combinations to find your playstyle",
		L"Tip: Trade items with other players to obtain gear that complements your character's strengths",
		L"Tip: Save valuable items in your stash for later use or trading with other players",
		L"Tip: Complete quests and challenges to earn rewards and experience points",
		L"Tip: Pay attention to the story and lore to fully immerse yourself in the world of Diablo 2",
		L"Tip: Use waypoints to quickly travel between different locations on the map",
		L"Tip: Upgrade your weapons and armor regularly to keep up with tougher enemies",
		L"Tip: Manage your inventory space efficiently to carry essential items and loot",
		L"Tip: Experiment with different strategies and tactics to overcome challenging boss fights",
		L"Tip: Keep an eye on your character's health and mana bars during battles",
		L"Tip: Take breaks between gaming sessions to avoid fatigue and maintain focus",
		L"Tip: Join online communities and forums to connect with other Diablo 2 players",
		L"Tip: Customize your hotkeys and controls for a more comfortable gaming experience",
		L"Tip: Experiment with different character classes to experience varied gameplay",
		L"Tip: Farm specific areas for rare items and loot to boost your character's power",
		L"Tip: Keep an eye on your character's resistances to mitigate elemental damage",
		L"Tip: Use crowd control skills to immobilize enemies and gain the upper hand in battle",
		L"Tip: Stock up on keys and potions before embarking on dungeon runs or boss fights",
		L"Tip: Pay attention to enemy attack patterns and weaknesses to devise effective strategies",
		L"Tip: Communicate with your teammates during multiplayer games to coordinate attacks and strategies",
		L"Tip: Explore different difficulty levels for increased challenges and rewards",
		L"Tip: Invest in your character's skills and abilities to enhance their effectiveness in combat",
		L"Tip: Experiment with different weapon and armor combinations to find the best setup for your playstyle",
		L"Tip: Keep an eye on the minimap for important locations and points of interest",
		L"Tip: Use the /players command to adjust the difficulty and loot drops in single-player games",
		L"Tip: Don't be afraid to retreat and regroup if you're overwhelmed by enemies",
		L"Tip: Pay attention to synergies between skills to maximize their effectiveness",
		L"Tip: Experiment with different character builds to find the one that suits your playstyle best",
		L"Tip: Take advantage of shrine effects to gain temporary bonuses and buffs",
		L"Tip: Use mercenaries to supplement your character's abilities and provide additional firepower",
		L"Tip: Explore dungeons and caves for hidden treasures and rare monsters",
		L"Tip: Keep an eye out for unique monsters with special abilities and loot drops",
		L"Tip: Experiment with different rune combinations to create powerful rune words for your gear",
		L"Tip: Keep an eye on your character's equipment durability and repair it when necessary",
		L"Tip: Use town portals strategically to escape dangerous situations or restock on supplies",
		L"Tip: Pay attention to your character's movement speed and agility to navigate obstacles and terrain efficiently",
		L"Tip: Learn and memorize monster spawn locations and behavior patterns for efficient farming",
		L"Tip: Take advantage of synergistic skills and abilities to amplify your character's strengths",
		L"Tip: Manage your gold wisely and invest in essential items and upgrades for your character",
		L"Tip: Experiment with different weapon types to find the one that best complements your character's skills",
		L"Tip: Utilize crowd control skills like stuns and freezes to control enemy movements during battles",
		L"Tip: Use the mercenary's aura skills to provide buffs and support to your character and party members",
		L"Tip: Pay attention to enemy resistances and vulnerabilities to choose the most effective attacks and spells",
		L"Tip: Keep an eye on your character's experience bar and level up regularly to access new skills and abilities",
		L"Tip: Experiment with different playstyles, such as melee, ranged, or magic, to find the one that suits you best",
		L"Tip: Use charms and jewels to customize your character's stats and abilities for specific challenges",
		L"Tip: Pay attention to your character's resistances and gear attributes to optimize your defense against various enemy types",
		L"Tip: Take advantage of environmental hazards like exploding barrels and traps to damage groups of enemies at once",
		L"Tip: Experiment with different potion combinations for healing, mana restoration, and temporary buffs",
		L"Tip: Utilize your character's class-specific skills and abilities to gain an edge in combat",
		L"Tip: Use debuff skills and abilities to weaken enemies and make them easier to defeat",
		L"Tip: Pay attention to your character's energy management to avoid running out of mana during battles",
		L"Tip: Utilize your character's movement skills to evade enemy attacks and reposition during battles",
		L"Tip: Take advantage of enemy weaknesses and vulnerabilities to deal extra damage during battles",
		L"Tip: Use traps and environmental hazards to your advantage during battles and skirmishes",
		L"Tip: Pay attention to enemy attack animations to anticipate and dodge incoming attacks",
		L"Tip: Keep an eye on your character's health and mana reserves during prolonged battles and encounters",
		L"Tip: Use consumable items like scrolls and potions strategically to gain temporary advantages in battles",
		L"Tip: Take advantage of the game's pause function to strategize and plan your next move during battles",
		L"Tip: Experiment with different weapon and armor upgrades to enhance your character's combat capabilities",
		L"Tip: Use your character's unique abilities and spells to overcome challenging obstacles and enemies",
		L"Tip: Keep an eye out for rare and unique items that can significantly boost your character's power and abilities",
		L"Tip: Use crowd control abilities like stuns and slows to control enemy movements and gain the upper hand in battles",
		L"Tip: Pay attention to enemy behavior and attack patterns to anticipate and counter their moves effectively",
		L"Tip: Utilize the game's auto-map feature to navigate dungeons and explore new areas efficiently",
		L"Tip: Experiment with different character builds and skill combinations to find the one that suits your playstyle best",
		L"Tip: Take advantage of synergies between your character's skills and abilities to maximize their effectiveness in combat",
		L"Tip: Keep an eye on your character's health and mana reserves during intense battles and encounters",
		L"Tip: Use your character's movement skills to dodge enemy attacks and reposition during battles",
		L"Tip: Pay attention to your character's energy management to ensure a steady supply of mana during battles",
		L"Tip: Use consumable items like potions and scrolls strategically to gain temporary advantages in battles",
		L"Tip: Take advantage of environmental hazards like exploding barrels and traps to damage groups of enemies at once",
		L"Tip: Keep an eye out for rare and unique items that can significantly enhance your character's power and abilities",
		L"Tip: Experiment with different weapon and armor upgrades to improve your character's combat capabilities",
		L"Tip: Use crowd control abilities like stuns and slows to control enemy movements and gain the upper hand in battles",
		L"Tip: Pay attention to enemy behavior and attack patterns to anticipate and counter their moves effectively",
		L"Tip: Utilize the game's auto-map feature to navigate dungeons and explore new areas efficiently",
		L"Tip: Experiment with different character builds and skill combinations to find the one that suits your playstyle best",
		L"Tip: Take advantage of synergies between your character's skills and abilities to maximize their effectiveness in combat",
		L"Tip: Keep an eye on your character's health and mana reserves during intense battles and encounters",
		L"Tip: Use your character's movement skills to dodge enemy attacks and reposition during battles",
		L"Tip: Pay attention to your character's energy management to ensure a steady supply of mana during battles",
		L"Tip: Use consumable items like potions and scrolls strategically to gain temporary advantages in battles",
		L"Tip: Take advantage of environmental hazards like exploding barrels and traps to damage groups of enemies at once",
		L"Tip: Keep an eye out for rare and unique items that can significantly enhance your character's power and abilities",
		L"Tip: Experiment with different weapon and armor upgrades to improve your character's combat capabilities",
		L"Tip: Use crowd control abilities like stuns and slows to control enemy movements and gain the upper hand in battles",
		L"Tip: Pay attention to enemy behavior and attack patterns to anticipate and counter their moves effectively",
		L"Tip: Utilize the game's auto-map feature to navigate dungeons and explore new areas efficiently",
		L"Tip: Experiment with different character builds and skill combinations to find the one that suits your playstyle best",
		L"Tip: Take advantage of synergies between your character's skills and abilities to maximize their effectiveness in combat",
		L"Tip: Keep an eye on your character's health and mana reserves during intense battles and encounters",
		L"Tip: Use your character's movement skills to dodge enemy attacks and reposition during battles",
		L"Tip: Pay attention to your character's energy management to ensure a steady supply of mana during battles",
		L"Tip: Use consumable items like potions and scrolls strategically to gain temporary advantages in battles",
		L"Tip: Take advantage of environmental hazards like exploding barrels and traps to damage groups of enemies at once",
		L"Tip: Keep an eye out for rare and unique items that can significantly enhance your character's power and abilities",
		L"Tip: Experiment with different weapon and armor upgrades to improve your character's combat capabilities",
		L"Tip: Use crowd control abilities like stuns and slows to control enemy movements and gain the upper hand in battles",
		L"Tip: Pay attention to enemy behavior and attack patterns to anticipate and counter their moves effectively",
		L"Tip: Utilize the game's auto-map feature to navigate dungeons and explore new areas efficiently",
		L"Tip: Experiment with different character builds and skill combinations to find the one that suits your playstyle best",
		L"Tip: Take advantage of synergies between your character's skills and abilities to maximize their effectiveness in combat",
		L"Tip: Keep an eye on your character's health and mana reserves during intense battles and encounters",
		L"Tip: Use your character's movement skills to dodge enemy attacks and reposition during battles",
		L"Tip: Pay attention to your character's energy management to ensure a steady supply of mana during battles",
		L"Tip: Use consumable items like potions and scrolls strategically to gain temporary advantages in battles",
		L"Tip: Take advantage of environmental hazards like exploding barrels and traps to damage groups of enemies at once",
		L"Tip: Keep an eye out for rare and unique items that can significantly enhance your character's power and abilities",
		L"Tip: Experiment with different weapon and armor upgrades to improve your character's combat capabilities",
		L"Tip: Use crowd control abilities like stuns and slows to control enemy movements and gain the upper hand in battles",
		L"Tip: Pay attention to enemy behavior and attack patterns to anticipate and counter their moves effectively",
		L"Tip: Utilize the game's auto-map feature to navigate dungeons and explore new areas efficiently",
		L"Tip: Experiment with different character builds and skill combinations to find the one that suits your playstyle best",
		L"Tip: Take advantage of synergies between your character's skills and abilities to maximize their effectiveness in combat",
		L"Tip: Keep an eye on your character's health and mana reserves during intense battles and encounters",
		L"Tip: Use your character's movement skills to dodge enemy attacks and reposition during battles",
		L"Tip: Pay attention to your character's energy management to ensure a steady supply of mana during battles",
		L"Tip: Use consumable items like potions and scrolls strategically to gain temporary advantages in battles",
		L"Tip: Take advantage of environmental hazards like exploding barrels and traps to damage groups of enemies at once",
		L"Tip: Keep an eye out for rare and unique items that can significantly enhance your character's power and abilities",
		L"Tip: Experiment with different weapon and armor upgrades to improve your character's combat capabilities",
		L"Tip: Use crowd control abilities like stuns and slows to control enemy movements and gain the upper hand in battles",
		L"Tip: Pay attention to enemy behavior and attack patterns to anticipate and counter their moves effectively",
		L"Tip: Utilize the game's auto-map feature to navigate dungeons and explore new areas efficiently",
		L"Tip: Experiment with different character builds and skill combinations to find the one that suits your playstyle best",
		L"Tip: Take advantage of synergies between your character's skills and abilities to maximize their effectiveness in combat",
		L"Tip: Keep an eye on your character's health and mana reserves during intense battles and encounters",
		L"Tip: Use your character's movement skills to dodge enemy attacks and reposition during battles",
		L"Tip: Pay attention to your character's energy management to ensure a steady supply of mana during battles",
		L"Tip: Use consumable items like potions and scrolls strategically to gain temporary advantages in battles",
		L"Tip: Take advantage of environmental hazards like exploding barrels and traps to damage groups of enemies at once",
		L"Tip: Keep an eye out for rare and unique items that can significantly enhance your character's power and abilities",
		L"Tip: Experiment with different weapon and armor upgrades to improve your character's combat capabilities",
		L"Tip: Use crowd control abilities like stuns and slows to control enemy movements and gain the upper hand in battles",
		L"Tip: Pay attention to enemy behavior and attack patterns to anticipate and counter their moves effectively",
		L"Tip: Utilize the game's auto-map feature to navigate dungeons and explore new areas efficiently",
		L"Tip: Experiment with different character builds and skill combinations to find the one that suits your playstyle best",
		L"Tip: Take advantage of synergies between your character's skills and abilities to maximize their effectiveness in combat",
		L"Tip: Keep an eye on your character's health and mana reserves during intense battles and encounters",
		L"Tip: Use your character's movement skills to dodge enemy attacks and reposition during battles",
		L"Tip: Pay attention to your character's energy management to ensure a steady supply of mana during battles",
		L"Tip: Use consumable items like potions and scrolls strategically to gain temporary advantages in battles",
		L"Tip: Take advantage of environmental hazards like exploding barrels and traps to damage groups of enemies at once",
		L"Tip: Keep an eye out for rare and unique items that can significantly enhance your character's power and abilities",
		L"Tip: Experiment with different weapon and armor upgrades to improve your character's combat capabilities",
		L"Tip: Use crowd control abilities like stuns and slows to control enemy movements and gain the upper hand in battles",

	};


	void OnLoad() {
		srand(time(NULL));
	}

	void OnDraw() {
		if (GetTickCount64() >= nEndTime) {
			nEndTime = GetTickCount64() + DURATION;
			nTip = rand() % diablo2Tips.size();
		}
		diablo2::d2_win::set_current_font(diablo2::UI_FONT_6);
		int sW = 1280;
		int sH = 768;

		int halfTextWidth = diablo2::d2_win::get_text_pixel_width(const_cast<wchar_t*>(diablo2Tips[nTip].c_str())) / 2;

		// Calculate the center point on the screen
		int centerX = sW / 2 - halfTextWidth;
		int centerY = sH / 2; // Subtract half the height of your desired y offset (50 in this case)

		if (!diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CHARACTER)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INVENTORY)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_SKILL)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CHAT)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_NPCMENU)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MAINMENU)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CONFIG)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_NPCSHOP)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_ANVIL)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_QUEST)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_QUESTLOG)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STATUSAREA)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_WPMENU)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_PARTY)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_TRADE)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MSGS)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_BELT)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_HELP)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MERC)
			&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_SCROLL)) {

			
			diablo2::d2_win::draw_text(const_cast<wchar_t*>(diablo2Tips[nTip].c_str()), centerX, 700, diablo2::UI_COLOR_GREY, 0);
		}
	}



	void draw() override {
		auto stats = globalStatsVector;
		int textOffset = 40; // Initial offset for the first line

		const auto player = diablo2::d2_client::get_local_player();

		// Add all items to vector
		std::vector<diablo2::structures::unit*> items;
		for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
			const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
			if (record->type == 104 || record->type == 123) {
				items.push_back(item);
			}
		}

		// Initialize statValue
		int32_t statValue = 0;

		for (const auto& stat : stats) {

			double param = 6;

			int32_t spirits = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(185), NULL);
			int32_t soulscaptured = statValue = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(184), NULL);

			switch (stat.stat) {
				// 2. (statValue <- this is probably op stat1 ? * baseValue <- this is probably op base ) / 2 ^ param

				// (op stat1 value *  base stat value) / (2 ^ param) 
				// let's try this fucking thing

			case 190: {
				// str/spirits
				statValue = static_cast<int32_t>((1 * spirits) / pow(2, param)); // what is the value of opStat_str
				break;
			}
			case 191: {
				// dex/spirits
				statValue = static_cast<int32_t>((1 * spirits) / pow(2, param)); // what is the value of opStat_str
				break;
			}
			case 192: {
				// vit/spirits
				statValue = static_cast<int32_t>((1 * spirits) / pow(2, param)); // what is the value of opStat_str
				break;
			}
			case 193: {
				// enr/spirits
				statValue = static_cast<int32_t>((1 * spirits) / pow(2, param)); // what is the value of opStat_str
				break;
			}
			case 200: {
				// skills/souls
				param = 8;
				statValue = static_cast<int32_t>((1 * soulscaptured) / pow(2, param)); // what is the value of opStat_str
				break;
			}

			case 301: {
				for (auto item : items) {
					const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
					if (record->type == 104) {
						statValue = diablo2::d2_common::get_stat(item, static_cast<diablo2::unit_stats_t>(stat.stat), NULL);
					}
				}
				break;
			}

			case 304: {
				for (auto item : items) {
					const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
					if (record->type == 104) {
						statValue = diablo2::d2_common::get_stat(item, static_cast<diablo2::unit_stats_t>(stat.stat), NULL);
					}
				}
				break;
			}

			default: {
				// By default, get player stats
				statValue = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(stat.stat), NULL);
				break;

			}
			}

			/*
			int32_t diablo2::d2_common::set_stat(structures::unit* unit, unit_stats_t stat, uint32_t value, int16_t param) {
					static wrap_func_std_import<int32_t(structures::unit*, int32_t, int32_t, int32_t)> set_stat(10517, get_base());
					return set_stat(unit, stat, value, param);
				}

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(randStatRangeLow, randStatRangeHigh);
			unsigned int randomNumber = dis(gen);

			std::random_device rdb;
			std::mt19937 genb(rdb());
			std::uniform_int_distribution<> randBool(1, 2);
			unsigned int randomBool = randBool(genb) - 1;

			if (stat.is_item_stat == 1) {
				for (auto item : items) {
					const auto record = diablo2::d2_common::get_item_record(item->data_record_index);
					int RandStatValue = diablo2::d2_common::get_stat(item, static_cast<diablo2::unit_stats_t>(randStat), NULL);

					if (record->type == stat.item_type_id && RandStatValue != 0) {
						// set randStat value to random number 1 and 2^(32) = 4294967296
						diablo2::d2_common::set_stat(item, static_cast<diablo2::unit_stats_t>(randStat), randomNumber, 0);
						diablo2::d2_common::set_stat(item, static_cast<diablo2::unit_stats_t>(randStatBool), randomBool, 0);
					}
				}
			}
			else {
				// set randStat value to random number 1 and 2^(32) = 4294967296
				//diablo2::d2_common::set_stat(player, static_cast<diablo2::unit_stats_t>(randStat), randomNumber, 0);
				//diablo2::d2_common::set_stat(player, static_cast<diablo2::unit_stats_t>(randStatBool), randomBool, 0);

				int statValue1 = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(randStat), NULL);
				int statValue2 = diablo2::d2_common::get_stat(player, static_cast<diablo2::unit_stats_t>(randStatBool), NULL);

				if (statValue1 > 0 ) {
					diablo2::d2_common::set_stat(player, static_cast<diablo2::unit_stats_t>(randStat), 0, 0);
					diablo2::d2_common::set_stat(player, static_cast<diablo2::unit_stats_t>(randStatBool), 0, 0);
				}

			}
			*/
			auto statValueStr = std::to_wstring(statValue);
			std::wstring statText = std::wstring(stat.stat_display_string);// .append(L" " + statValueStr);

			if (!diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CHARACTER)
				// && !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INVENTORY)
				// && !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_SKILL)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CHAT)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_NPCMENU)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MAINMENU)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CONFIG)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_NPCSHOP)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_ANVIL)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_QUEST)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_QUESTLOG)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STATUSAREA)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_WPMENU)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_PARTY)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_TRADE)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MSGS)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_BELT)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_HELP)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_MERC)
				&& !diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_SCROLL)) {

				// Draw stats
				diablo2::d2_win::set_current_font(diablo2::UI_FONT_6); // Set font to FONT16
				diablo2::d2_win::draw_text(const_cast<wchar_t*>(statText.c_str()), stat.x1, stat.y1 + textOffset, stat.colorStat, 0);

				diablo2::d2_win::set_current_font(diablo2::UI_FONT_6); // Set font to FONT16
				diablo2::d2_win::draw_text(const_cast<wchar_t*>(statValueStr.c_str()), stat.x2, stat.y2 + textOffset, stat.colorStatValue, 0);


				//diablo2::d2_win::draw_boxed_text(const_cast<wchar_t*>(statText.c_str()), stat.x1, stat.y1 + textOffset, 1, 0, stat.colorStat);
				//diablo2::d2_win::draw_boxed_text(const_cast<wchar_t*>(statValueStr.c_str()), stat.x2, stat.y2 + textOffset, 1, 1, statValueColor);

				// diablo2::d2_win::set_current_font(diablo2::UI_FONT_16); // Set font to FONT16


				//diablo2::structures::d2_cmp::init_gfx_data(&g_gfxdata);

				//diablo2::d2_gfx::draw_image(&g_gfxdata, 200, 200, 1, 5, 0);

				// instead try to load direct jpg with gdi and insetad ofloading jpg file, specify it bb64 encoded and decode it.

				diablo2::ui_color_t::UI_COLOR_WHITE;



				OnDraw();



			}
		}

		if (!should_draw()) {
			m_sort_inventory_btn->set_enabled(false);
			m_sort_inventory_btn->set_visible(false);

			m_sort_stash_btn->set_enabled(false);
			m_sort_stash_btn->set_visible(false);

			m_sort_cube_btn->set_enabled(false);
			m_sort_cube_btn->set_visible(false);

			return;
		}

		m_sort_inventory_btn->set_enabled(true);
		m_sort_inventory_btn->set_visible(true);

		m_sort_stash_btn->set_enabled(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH));
		m_sort_stash_btn->set_visible(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH));

		m_sort_cube_btn->set_enabled(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE));
		m_sort_cube_btn->set_visible(diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE));

		menu::draw();
	}
private:
	static bool should_draw() {
		return diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_INVENTORY) ||
			diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_STASH) ||
			diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_CUBE) ||
			diablo2::d2_client::get_ui_window_state(diablo2::UI_WINDOW_NPCSHOP);
	}

	d2_tweaks::ui::controls::button* get_button(const std::string& name, const std::function<void()>& onClick) {
		auto result = static_cast<d2_tweaks::ui::controls::button*>(get_control(name));

		result->set_on_click(onClick);

		result->set_enabled(false);
		result->set_visible(false);

		return result;
	}

	void sort_inventory_click() {
		request_sort(0x00);
	}

	void sort_stash_click() {
		request_sort(0x04);
	}

	void sort_cube_click() {
		request_sort(0x03);
	}

	void request_sort(const uint8_t page) {
		static d2_tweaks::common::inventory_sort_cs packet;

		const auto player = diablo2::d2_client::get_local_player();

		std::vector<diablo2::structures::unit*> items;

		for (auto item = player->inventory->first_item; item != nullptr; item = item->item_data->pt_next_item) {
			if (item->item_data->page == page)
				items.push_back(item);
		}

		for (auto item : items)
			diablo2::d2_common::inv_remove_item(player->inventory, item);

		packet.page = page;
		diablo2::d2_client::send_to_server(&packet, sizeof packet);
	}
};


void d2_tweaks::client::modules::autosort::init_early() {

}


void d2_tweaks::client::modules::autosort::init() {
	char acPathToIni[MAX_PATH] = { 0 };
	const char* pcIniFile = "\\d2tweaks.ini";

	GetCurrentDirectory(MAX_PATH, acPathToIni);
	lstrcat(acPathToIni, pcIniFile);

	if (GetPrivateProfileInt("modules", "Autosort", 1, acPathToIni) != FALSE) {
		singleton<ui::ui_manager>::instance().add_menu(new inventory_sort_menu());
		singleton<client>::instance().register_packet_handler(common::MESSAGE_TYPE_INVENTORY_SORT, this);
	}
}

void d2_tweaks::client::modules::autosort::handle_packet(common::packet_header* packet) {
	static auto& instance = singleton<client>::instance();
	const auto inventorySort = static_cast<common::inventory_sort_sc*>(packet);

	const auto item = instance.get_client_unit(0x04, inventorySort->guid); //0x03 -> 0x04 - item

	if (item == nullptr)
		return;;

	const auto player = diablo2::d2_client::get_local_player();

	//Last parameter is some boolean
	const auto inventoryIndex = diablo2::d2_common::get_inventory_index(player, inventorySort->page, diablo2::d2_client::is_lod());

	item->item_data->page = inventorySort->page;

	diablo2::d2_common::inv_add_item(player->inventory, item, inventorySort->tx, inventorySort->ty, inventoryIndex, true, item->item_data->page);
	diablo2::d2_common::inv_update_item(player->inventory, item, true);
}

