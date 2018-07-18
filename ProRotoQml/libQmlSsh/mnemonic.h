/**
 * libmnemonic v1.1
 */

#ifndef MNEMONIC_H_
#define MNEMONIC_H_

#include <cstdint>
using namespace std;

/**
 * Mnemonic key format.
 *
 * The mnemonic phrase is generated from binary data "entropy". The serialization
 * format is the one used in BIP0039.
 */
#define MNEMONIC_SALT                       ("mnemonic")
#define MNEMONIC_DEFAULT_LENGTH             (16)
#define MNEMONIC_PBKDF2_ITERATIONS          (2048)
#define SEED_SIZE                           (32)

typedef vector<string> mnemonic_words;
typedef vector<uint8_t> bytes;
typedef vector<uint8_t> bits;


typedef unsigned int uint;
/**
 * Generate a mnemonic from the provided entropy.
 *
 * Parameters:
 *  randomData - an array that contains the random data
 *  length - the length of the random data, must be multiple of 4 bytes
 *
 * Security Notice: It is the responsibility of the caller to
 * provide cryptographically secure entropy.
 */
string createMnemonic(const uint8_t *randomData,
        uint length = MNEMONIC_DEFAULT_LENGTH);

/**
 * Generate a seed from the provided mnemonic
 */
bytes mnemonicToSeed(const string& mnemonic);

/**
 * Exceptions
 */
class MnemonicException : public exception {
    const string errMessage;
public:
    MnemonicException(const string message):errMessage(message){}
    const char* what() const throw() { return errMessage.c_str(); }
};

class MnemonicChecksumException: public MnemonicException {
public:
    MnemonicChecksumException(const string message):MnemonicException(message){}
};

class MnemonicLengthException: public MnemonicException {
public:
    MnemonicLengthException(const string message):MnemonicException(message){}
};

class MnemonicWordException: public MnemonicException {
public:
    MnemonicWordException(const string message):MnemonicException(message){}
};

const mnemonic_words wordlist = {
    "abandon",
    "ability",
    "accross",
    "accuse",
    "acheive",
    "acquire",
    "actual",
    "addition",
    "adjust",
    "admit",
    "adrian",
    "advance",
    "advertise",
    "advice",
    "afl",
    "again",
    "agent",
    "ahead",
    "aikman",
    "aim",
    "air",
    "alert",
    "align",
    "allen",
    "allow",
    "almost",
    "alworth",
    "amateur",
    "amazing",
    "amount",
    "analyst",
    "anchor",
    "angle",
    "angry",
    "announce",
    "antenna",
    "appear",
    "arch",
    "arena",
    "arizona",
    "arrange",
    "assail",
    "asset",
    "atkins",
    "atlanta",
    "attack",
    "attempt",
    "auction",
    "auto",
    "average",
    "avoid",
    "aware",
    "away",
    "baby",
    "bachelor",
    "backup",
    "bacon",
    "bacteria",
    "badger",
    "ball",
    "baltimore",
    "bar",
    "barbecue",
    "bargain",
    "barney",
    "barry",
    "basic",
    "bat",
    "battle",
    "battles",
    "bears",
    "behind",
    "belichick",
    "bell",
    "below",
    "bench",
    "bengals",
    "berry",
    "bettis",
    "bid",
    "bills",
    "black",
    "blanda",
    "blast",
    "blaze",
    "blend",
    "blind",
    "blip",
    "blitz",
    "block",
    "blount",
    "blowout",
    "bolt",
    "bomb",
    "bonus",
    "boomber",
    "boost",
    "bootleg",
    "border",
    "bound",
    "bowl",
    "boy",
    "bracket",
    "bradshaw",
    "brady",
    "brett",
    "bring",
    "brisk",
    "broken",
    "broncos",
    "brooks",
    "brown",
    "buccaneers",
    "buchanan",
    "buffalo",
    "build",
    "bulk",
    "bump",
    "business",
    "bust",
    "butkus",
    "butler",
    "buzz",
    "call",
    "campbell",
    "cancel",
    "capable",
    "captain",
    "cardinals",
    "career",
    "carolina",
    "carry",
    "carson",
    "carter",
    "cascade",
    "cash",
    "casper",
    "catch",
    "category",
    "caution",
    "cave",
    "center",
    "certain",
    "chain",
    "chalk",
    "champion",
    "chargers",
    "chase",
    "cheat",
    "check",
    "cheer",
    "cheese",
    "chicago",
    "chiefs",
    "chin",
    "choice",
    "choose",
    "chop",
    "chuck",
    "cinci",
    "city",
    "clark",
    "clear",
    "cleveland",
    "climb",
    "clip",
    "clog",
    "close",
    "clutch",
    "coach",
    "coast",
    "code",
    "coffin",
    "colts",
    "column",
    "comeback",
    "commit",
    "complete",
    "concuss",
    "conduct",
    "conference",
    "connor",
    "controversy",
    "convert",
    "cooler",
    "corner",
    "correct",
    "cost",
    "couch",
    "count",
    "course",
    "cover",
    "cowboys",
    "craft",
    "creative",
    "crew",
    "cross",
    "crowd",
    "crucial",
    "cruise",
    "culp",
    "cup",
    "curtis",
    "curve",
    "cut",
    "dallas",
    "damage",
    "dance",
    "danger",
    "daring",
    "dart",
    "dash",
    "daughter",
    "davis",
    "dawn",
    "dawson",
    "day",
    "deal",
    "dean",
    "debate",
    "debris",
    "decade",
    "deception",
    "decide",
    "decline",
    "decorate",
    "decrease",
    "deep",
    "deer",
    "defense",
    "define",
    "deflate",
    "defy",
    "degree",
    "delay",
    "deliver",
    "demand",
    "demise",
    "denial",
    "dent",
    "denver",
    "deny",
    "depart",
    "depend",
    "deposit",
    "depth",
    "deputy",
    "derive",
    "describe",
    "desert",
    "design",
    "desk",
    "despair",
    "destroy",
    "detail",
    "detect",
    "detroit",
    "develop",
    "device",
    "devote",
    "diagram",
    "dial",
    "diamond",
    "diary",
    "dice",
    "dickerson",
    "dierdorf",
    "diesel",
    "diet",
    "differ",
    "different",
    "digital",
    "dignity",
    "dilemma",
    "dime",
    "dinner",
    "dino",
    "direct",
    "dirt",
    "disagree",
    "discover",
    "disease",
    "dish",
    "dismiss",
    "disorder",
    "display",
    "distance",
    "ditka",
    "dive",
    "divert",
    "divide",
    "divorce",
    "dizzy",
    "doctor",
    "document",
    "dog",
    "dogs",
    "doleman",
    "doll",
    "dolphins",
    "domain",
    "donate",
    "donkey",
    "donovan",
    "door",
    "dorsett",
    "dose",
    "double",
    "dove",
    "down",
    "draft",
    "dragon",
    "drama",
    "drastic",
    "draw",
    "dream",
    "dress",
    "drift",
    "drill",
    "drink",
    "drip",
    "drive",
    "drop",
    "drug",
    "drum",
    "dry",
    "duck",
    "dudley",
    "dumb",
    "dune",
    "during",
    "dust",
    "dutch",
    "duty",
    "dwarf",
    "dynamic",
    "eager",
    "eagles",
    "early",
    "earn",
    "earth",
    "easily",
    "east",
    "easy",
    "echo",
    "ecology",
    "economy",
    "edge",
    "edit",
    "educate",
    "edwards",
    "effort",
    "egg",
    "eight",
    "either",
    "elbow",
    "elder",
    "electric",
    "elegant",
    "element",
    "elephant",
    "elevator",
    "elite",
    "eller",
    "else",
    "elway",
    "embark",
    "embody",
    "embrace",
    "emerge",
    "emotion",
    "emply",
    "empower",
    "empty",
    "enable",
    "enact",
    "encroach",
    "end",
    "endless",
    "endorse",
    "enemy",
    "energy",
    "enforce",
    "engage",
    "engine",
    "enhance",
    "enjoy",
    "enlist",
    "enough",
    "enrich",
    "enroll",
    "ensure",
    "enter",
    "entire",
    "entry",
    "envelope",
    "episode",
    "equal",
    "equipment",
    "era",
    "erase",
    "erode",
    "erosion",
    "error",
    "erupt",
    "escape",
    "essay",
    "essence",
    "estate",
    "eternal",
    "ethics",
    "evidence",
    "evil",
    "evoke",
    "evolve",
    "ewbank",
    "exact",
    "example",
    "excess",
    "exchange",
    "excite",
    "exclude",
    "excuse",
    "execute",
    "exercise",
    "exhaust",
    "exhibit",
    "exile",
    "exist",
    "exit",
    "exotic",
    "expand",
    "expect",
    "experience",
    "expire",
    "explain",
    "expose",
    "express",
    "extend",
    "extra",
    "eye",
    "eyebrow",
    "fabric",
    "face",
    "faculty",
    "fade",
    "faint",
    "fair",
    "faith",
    "falcons",
    "fall",
    "fame",
    "family",
    "famous",
    "fan",
    "fancy",
    "fantasy",
    "farm",
    "fashion",
    "fat",
    "fatal",
    "father",
    "fatigue",
    "faulk",
    "favorite",
    "favre",
    "fears",
    "feature",
    "february",
    "federal",
    "fee",
    "feed",
    "feel",
    "female",
    "fence",
    "festival",
    "fetch",
    "fever",
    "few",
    "fiber",
    "fiction",
    "field",
    "figure",
    "file",
    "film",
    "filter",
    "final",
    "find",
    "fine",
    "finger",
    "finish",
    "finks",
    "fire",
    "firm",
    "first",
    "fiscal",
    "fish",
    "fit",
    "fitness",
    "fix",
    "flag",
    "flame",
    "flanker",
    "flash",
    "flat",
    "flavor",
    "flee",
    "flex",
    "flight",
    "flinch",
    "flip",
    "float",
    "flock",
    "floor",
    "flower",
    "fluid",
    "flush",
    "fly",
    "foam",
    "focus",
    "fog",
    "foil",
    "fold",
    "follow",
    "food",
    "fool",
    "foot",
    "football",
    "force",
    "ford",
    "forest",
    "forget",
    "fork",
    "formation",
    "fortmann",
    "fortune",
    "forum",
    "forward",
    "fossil",
    "foster",
    "foul",
    "found",
    "fouts",
    "fox",
    "fragile",
    "frame",
    "free",
    "frequent",
    "freshman",
    "friedman",
    "fringe",
    "frog",
    "front",
    "frost",
    "frown",
    "frozen",
    "fruit",
    "fuel",
    "fullback",
    "fumble",
    "fun",
    "funny",
    "furnace",
    "fury",
    "future",
    "gadget",
    "gain",
    "galaxy",
    "gallery",
    "gallop",
    "game",
    "gap",
    "garage",
    "garbage",
    "garden",
    "garlic",
    "garment",
    "gas",
    "gasp",
    "gate",
    "gather",
    "gatorade",
    "gatski",
    "gauge",
    "gauze",
    "gaze",
    "general",
    "genius",
    "genre",
    "gentle",
    "genuine",
    "george",
    "gesture",
    "ghost",
    "giant",
    "giants",
    "gibbs",
    "gifford",
    "gift",
    "giggle",
    "gillman",
    "ginger",
    "giraffe",
    "girl",
    "give",
    "glad",
    "glance",
    "glare",
    "glass",
    "glide",
    "glimpse",
    "globe",
    "gloom",
    "glory",
    "glove",
    "glow",
    "glue",
    "go",
    "goal",
    "goat",
    "goddess",
    "gold",
    "good",
    "goose",
    "gorilla",
    "gospel",
    "gossip",
    "govern",
    "gown",
    "grab",
    "grace",
    "graham",
    "grain",
    "grant",
    "grape",
    "grass",
    "gravity",
    "great",
    "green",
    "gregg",
    "gridiron",
    "grief",
    "griese",
    "grimm",
    "grit",
    "grocery",
    "grounding",
    "group",
    "grow",
    "groza",
    "grunt",
    "guard",
    "guess",
    "guide",
    "guilt",
    "gunner",
    "gunslinger",
    "gush",
    "guy",
    "guyon",
    "gym",
    "habit",
    "halas",
    "haley",
    "halfback",
    "hall",
    "hamburger",
    "hampton",
    "hand",
    "hannah",
    "happen",
    "harris",
    "hash",
    "hayes",
    "haynes",
    "head",
    "healey",
    "health",
    "heart",
    "heavy",
    "hedgehog",
    "height",
    "heinz",
    "hello",
    "helmet",
    "help",
    "hen",
    "hendricks",
    "henry",
    "herber",
    "hero",
    "hewitt",
    "hickerson",
    "hidden",
    "high",
    "hike",
    "hill",
    "hinkle",
    "hint",
    "hip",
    "hire",
    "hirsch",
    "history",
    "hit",
    "hobby",
    "hockey",
    "holding",
    "hole",
    "holiday",
    "hollow",
    "holt",
    "home",
    "honey",
    "hood",
    "hook",
    "hop",
    "hope",
    "horn",
    "horror",
    "horse",
    "hospital",
    "host",
    "hotel",
    "hour",
    "house",
    "houston",
    "hover",
    "hub",
    "hubbard",
    "huddle",
    "huff",
    "huge",
    "human",
    "humble",
    "humor",
    "hump",
    "hundred",
    "hungry",
    "hunt",
    "hurdle",
    "hurry",
    "hurt",
    "husband",
    "hutson",
    "hybrid",
    "ice",
    "icon",
    "idea",
    "identify",
    "idle",
    "ignore",
    "ill",
    "illegal",
    "illness",
    "image",
    "imitate",
    "immense",
    "immune",
    "impact",
    "impose",
    "improve",
    "impulse",
    "inbounds",
    "inch",
    "include",
    "income",
    "incomplete",
    "increase",
    "index",
    "indianapolis",
    "indicate",
    "indirect",
    "indoor",
    "industry",
    "ineligible",
    "infacnt",
    "infamous",
    "inflict",
    "inform",
    "infraction",
    "inhale",
    "inherit",
    "initial",
    "inject",
    "injury",
    "inmate",
    "inner",
    "innocent",
    "input",
    "inquiry",
    "insane",
    "insect",
    "inside",
    "inspire",
    "install",
    "instant",
    "intact",
    "intentional",
    "interception",
    "interest",
    "interior",
    "into",
    "invest",
    "invite",
    "involve",
    "iron",
    "irvin",
    "island",
    "isolate",
    "issue",
    "item",
    "ivory",
    "jacket",
    "jacksonville",
    "jaguar",
    "jaguars",
    "jammer",
    "jar",
    "jazz",
    "jealous",
    "jeans",
    "jelly",
    "jerk",
    "jets",
    "jewel",
    "job",
    "jog",
    "johnson",
    "join",
    "joiner",
    "joke",
    "jolt",
    "jones",
    "jordan",
    "journey",
    "joy",
    "judge",
    "juice",
    "juke",
    "jumbo",
    "jump",
    "jungle",
    "junior",
    "junk",
    "jurgensen",
    "just",
    "kangaroo",
    "kansas",
    "keen",
    "keep",
    "kelly",
    "kennedy",
    "ketchup",
    "key",
    "kick",
    "kid",
    "kidney",
    "kiesling",
    "kinard",
    "kind",
    "kingdom",
    "kiss",
    "kitchen",
    "kite",
    "kitten",
    "kiwi",
    "kneel",
    "knife",
    "knock",
    "know",
    "krause",
    "lab",
    "label",
    "labor",
    "ladder",
    "lady",
    "lake",
    "lambert",
    "lamp",
    "landry",
    "lane",
    "langer",
    "lanier",
    "laptop",
    "largent",
    "lary",
    "lateral",
    "latin",
    "laugh",
    "laundry",
    "lava",
    "lavelli",
    "law",
    "lawn",
    "lawsuit",
    "layer",
    "layne",
    "lazy",
    "leader",
    "leaf",
    "leaping",
    "leapord",
    "learn",
    "leave",
    "lecture",
    "leemans",
    "left",
    "leg",
    "legal",
    "legend",
    "leisure",
    "lemon",
    "lend",
    "length",
    "lens",
    "lesson",
    "letter",
    "level",
    "levy",
    "liar",
    "liberty",
    "library",
    "license",
    "life",
    "lift",
    "light",
    "like",
    "lilly",
    "limb",
    "limit",
    "line",
    "link",
    "lion",
    "lions",
    "liquid",
    "list",
    "little",
    "live",
    "lizard",
    "load",
    "loan",
    "lob",
    "lobster",
    "local",
    "lock",
    "lofton",
    "logic",
    "lombardi",
    "lonely",
    "long",
    "loop",
    "loose",
    "losing",
    "lott",
    "lottery",
    "loud",
    "lounge",
    "love",
    "loyal",
    "luckman",
    "lucky",
    "luggage",
    "lumber",
    "lunar",
    "lunch",
    "luxury",
    "lyman",
    "lyrics",
    "machine",
    "mack",
    "mad",
    "madden",
    "magic",
    "magnet",
    "maid",
    "mail",
    "maintain",
    "major",
    "make",
    "mammal",
    "man",
    "manager",
    "mandate",
    "mango",
    "mansion",
    "manual",
    "maple",
    "mara",
    "marble",
    "marchetti",
    "margin",
    "marino",
    "market",
    "marriage",
    "marshall",
    "martin",
    "marvin",
    "mask",
    "mass",
    "master",
    "match",
    "material",
    "math",
    "matrix",
    "matson",
    "matter",
    "matthews",
    "max",
    "maximum",
    "maynard",
    "maze",
    "mcafee",
    "mccormack",
    "mcdaniel",
    "mcdonald",
    "meadow",
    "mean",
    "measure",
    "meat",
    "mechanic",
    "medal",
    "media",
    "melody",
    "melt",
    "member",
    "memory",
    "mention",
    "menu",
    "mercy",
    "merge",
    "merit",
    "merry",
    "mesh",
    "message",
    "metal",
    "method",
    "metrodome",
    "miami",
    "middle",
    "midnight",
    "mike",
    "milk",
    "million",
    "millner",
    "mimic",
    "mind",
    "minimum",
    "minnesota",
    "minor",
    "mint",
    "minute",
    "miracle",
    "mirror",
    "misery",
    "miss",
    "mistake",
    "mitchell",
    "mix",
    "mixed",
    "mixture",
    "mobile",
    "model",
    "modern",
    "modification",
    "mom",
    "moment",
    "monitor",
    "monk",
    "monkey",
    "monster",
    "montana",
    "month",
    "moon",
    "moore",
    "moral",
    "more",
    "morning",
    "mosquito",
    "moss",
    "mother",
    "motion",
    "motley",
    "motor",
    "mountain",
    "mouse",
    "move",
    "movie",
    "much",
    "muff",
    "mule",
    "multiply",
    "munchak",
    "muscle",
    "museum",
    "mushroom",
    "music",
    "musso",
    "must",
    "mutual",
    "myself",
    "mystery",
    "myth",
    "nachos",
    "naive",
    "namath",
    "name",
    "napkin",
    "narrow",
    "nasty",
    "nation",
    "nature",
    "neale",
    "near",
    "neglect",
    "neither",
    "nephew",
    "nerve",
    "nest",
    "net",
    "network",
    "neutral",
    "nevers",
    "newsome",
    "next",
    "nfl",
    "nice",
    "night",
    "niners",
    "nitschke",
    "noble",
    "noise",
    "noll",
    "nominee",
    "normal",
    "nose",
    "notable",
    "note",
    "nothing",
    "notice",
    "novel",
    "now",
    "nuclear",
    "number",
    "nurse",
    "nut",
    "oak",
    "oakland",
    "oblige",
    "obscure",
    "occur",
    "offense",
    "official",
    "offside",
    "olsen",
    "omaha",
    "onside",
    "opal",
    "open",
    "opposing",
    "option",
    "organize",
    "otto",
    "outside",
    "oval",
    "oven",
    "overtime",
    "owens",
    "owner",
    "packers",
    "pads",
    "page",
    "paint",
    "pair",
    "pancake",
    "panthers",
    "parcells",
    "parker",
    "pass",
    "past",
    "patch",
    "path",
    "patient",
    "patriots",
    "pause",
    "payment",
    "payton",
    "peace",
    "peanut",
    "pearl",
    "peasant",
    "peel",
    "penalty",
    "percentage",
    "perry",
    "personal",
    "peyton",
    "philadelphia",
    "physical",
    "pick",
    "picnic",
    "pigskin",
    "pilot",
    "pioneer",
    "pipe",
    "pistol",
    "pit",
    "place",
    "plan",
    "platoon",
    "play",
    "plow",
    "pluck",
    "plug",
    "pocket",
    "points",
    "pole",
    "polian",
    "pollard",
    "pony",
    "pooch",
    "portion",
    "pose",
    "position",
    "post",
    "pounce",
    "pound",
    "predict",
    "prepare",
    "preseason",
    "pressure",
    "prevent",
    "price",
    "pride",
    "primary",
    "print",
    "priority",
    "prison",
    "private",
    "prize",
    "pro",
    "problem",
    "proceed",
    "produce",
    "profit",
    "program",
    "prohibit",
    "project",
    "promote",
    "proof",
    "property",
    "prosper",
    "protect",
    "proud",
    "provide",
    "public",
    "pudding",
    "pull",
    "pulp",
    "pulse",
    "pump",
    "punch",
    "punt",
    "pupil",
    "puppy",
    "purchase",
    "purity",
    "purpose",
    "purse",
    "push",
    "put",
    "puzzle",
    "pylon",
    "pyramid",
    "quality",
    "quantum",
    "quarterback",
    "question",
    "quick",
    "quit",
    "quiz",
    "quote",
    "rabbit",
    "raccoon",
    "race",
    "rack",
    "radar",
    "radio",
    "rag",
    "raiders",
    "rail",
    "rain",
    "raise",
    "rally",
    "ramp",
    "rams",
    "ranch",
    "randle",
    "randy",
    "range",
    "rapid",
    "rare",
    "rate",
    "rather",
    "ravens",
    "raw",
    "ray",
    "razor",
    "ready",
    "real",
    "reason",
    "rebel",
    "rebuild",
    "recall",
    "reception",
    "recipe",
    "recoil",
    "record",
    "recycle",
    "red",
    "redskins",
    "reduce",
    "reed",
    "reeves",
    "referee",
    "reflect",
    "reform",
    "refuse",
    "region",
    "regret",
    "regular",
    "reject",
    "relax",
    "relay",
    "release",
    "relief",
    "rely",
    "remain",
    "remember",
    "remind",
    "remove",
    "render",
    "renew",
    "renfro",
    "rent",
    "reopen",
    "repair",
    "repeat",
    "replay",
    "report",
    "require",
    "rescue",
    "research",
    "resemble",
    "resist",
    "resource",
    "response",
    "restrain",
    "result",
    "retire",
    "retreat",
    "return",
    "reunion",
    "reverse",
    "review",
    "reward",
    "rhythm",
    "ribbon",
    "rice",
    "richter",
    "ride",
    "ridge",
    "rifle",
    "riggins",
    "right",
    "rigid",
    "ringo",
    "riot",
    "ripple",
    "rise",
    "risk",
    "ritual",
    "rival",
    "river",
    "road",
    "roaf",
    "roast",
    "robinson",
    "robot",
    "robustelli",
    "rocket",
    "rodger",
    "role",
    "rollout",
    "romance",
    "romo",
    "roof",
    "rookie",
    "room",
    "rooney",
    "rose",
    "rotate",
    "roughing",
    "round",
    "route",
    "rover",
    "royal",
    "rozelle",
    "rubber",
    "rude",
    "rug",
    "rules",
    "run",
    "runway",
    "rural",
    "rush",
    "sabol",
    "sack",
    "sad",
    "saddle",
    "sadness",
    "safety",
    "sail",
    "saints",
    "salary",
    "salmon",
    "salon",
    "salt",
    "salute",
    "sam",
    "same",
    "sample",
    "sanders",
    "sapp",
    "satisfy",
    "satoshi",
    "sauce",
    "sausage",
    "save",
    "say",
    "sayers",
    "scale",
    "scamper",
    "scandal",
    "scare",
    "scatback",
    "scatter",
    "scene",
    "scheme",
    "schmidt",
    "school",
    "schramm",
    "science",
    "scissors",
    "scoop",
    "score",
    "scout",
    "scramble",
    "scrap",
    "screen",
    "scrimmage",
    "script",
    "scrub",
    "scurry",
    "sea",
    "seahawks",
    "search",
    "season",
    "seattle",
    "seau",
    "second",
    "secret",
    "section",
    "security",
    "seed",
    "seek",
    "segment",
    "select",
    "sell",
    "selmon",
    "seminar",
    "senior",
    "sensor",
    "sentence",
    "series",
    "service",
    "session",
    "settle",
    "setup",
    "seven",
    "shadow",
    "shafe",
    "shallow",
    "shape",
    "sharpe",
    "shaw",
    "shed",
    "shell",
    "sheriff",
    "shields",
    "shift",
    "shill",
    "shin",
    "ship",
    "shiver",
    "shock",
    "shoe",
    "shoot",
    "shop",
    "short",
    "shotgun",
    "shoulder",
    "shove",
    "shrimp",
    "shrug",
    "shuffle",
    "shula",
    "shy",
    "sibling",
    "sick",
    "side",
    "siege",
    "sight",
    "signals",
    "silent",
    "silk",
    "silly",
    "silver",
    "similar",
    "simple",
    "simpson",
    "since",
    "singletary",
    "siren",
    "sister",
    "situation",
    "six",
    "size",
    "skate",
    "sketch",
    "ski",
    "skill",
    "skin",
    "skirt",
    "skull",
    "skyrocket",
    "slab",
    "slam",
    "slant",
    "slater",
    "sleeper",
    "slender",
    "slice",
    "slide",
    "slight",
    "slim",
    "slogan",
    "slot",
    "slow",
    "slush",
    "smack",
    "small",
    "smart",
    "smile",
    "smith",
    "smoke",
    "smooth",
    "snake",
    "snap",
    "sneak",
    "sniff",
    "snow",
    "soap",
    "soar",
    "soccer",
    "social",
    "sock",
    "soda",
    "soft",
    "soil",
    "solar",
    "soldier",
    "solid",
    "solution",
    "solve",
    "someone",
    "song",
    "soon",
    "sophomore",
    "sorry",
    "soul",
    "sound",
    "soup",
    "source",
    "south",
    "space",
    "spare",
    "spatial",
    "spawn",
    "special",
    "speed",
    "spell",
    "spend",
    "sphere",
    "spice",
    "spider",
    "spike",
    "spin",
    "spiral",
    "spirit",
    "split",
    "spoil",
    "sponsor",
    "spoon",
    "sport",
    "spot",
    "spray",
    "spread",
    "sprint",
    "spy",
    "spygate",
    "square",
    "squeeze",
    "squib",
    "squirrel",
    "stable",
    "stadium",
    "staff",
    "stage",
    "stairs",
    "stallworth",
    "stamp",
    "standard",
    "stare",
    "starr",
    "starter",
    "state",
    "stay",
    "steal",
    "steelers",
    "stem",
    "stereo",
    "sticks",
    "stiff",
    "still",
    "sting",
    "stock",
    "stomach",
    "stone",
    "stool",
    "stop",
    "story",
    "stove",
    "strahan",
    "straight",
    "stram",
    "streak",
    "stretch",
    "string",
    "strip",
    "strong",
    "structure",
    "student",
    "stuff",
    "stumble",
    "stunt",
    "stydahar",
    "style",
    "subject",
    "submit",
    "substitution",
    "subway",
    "success",
    "such",
    "suddenly",
    "suffer",
    "sugar",
    "suggest",
    "suicide",
    "suit",
    "summer",
    "sun",
    "sunny",
    "sunset",
    "super",
    "supply",
    "supreme",
    "sure",
    "surface",
    "surge",
    "surprise",
    "surround",
    "survey",
    "suspect",
    "sustain",
    "swallow",
    "swamp",
    "swann",
    "swap",
    "swarm",
    "swear",
    "sweep",
    "swift",
    "swim",
    "swing",
    "switch",
    "sword",
    "symbol",
    "symptom",
    "syrup",
    "system",
    "tabasco",
    "tabby",
    "table",
    "tabulate",
    "tackle",
    "tact",
    "tag",
    "tailback",
    "take",
    "talent",
    "talk",
    "tang",
    "tank",
    "tape",
    "tar",
    "target",
    "tarkenton",
    "tarot",
    "task",
    "taste",
    "tattoo",
    "tax",
    "taxi",
    "taylor",
    "teach",
    "team",
    "television",
    "tell",
    "temper",
    "ten",
    "tenant",
    "tend",
    "tennessee",
    "tent",
    "term",
    "terminate",
    "terrain",
    "terrell",
    "test",
    "texans",
    "text",
    "thank",
    "that",
    "theme",
    "then",
    "theory",
    "there",
    "they",
    "thing",
    "third",
    "this",
    "thomas",
    "thorpe",
    "thought",
    "three",
    "thrive",
    "throw",
    "thumb",
    "thunder",
    "ticket",
    "tide",
    "tiger",
    "tilt",
    "timber",
    "time",
    "tingle",
    "tiny",
    "tip",
    "tippett",
    "tired",
    "tissue",
    "titans",
    "tithe",
    "title",
    "tittle",
    "toast",
    "tobacco",
    "today",
    "toddler",
    "toe",
    "together",
    "toilet",
    "token",
    "tomato",
    "tomlinson",
    "tomorrow",
    "tone",
    "tongue",
    "tonight",
    "tony",
    "tool",
    "tooth",
    "top",
    "topic",
    "topple",
    "torch",
    "tornado",
    "torpedo",
    "torry",
    "tortoise",
    "torture",
    "toss",
    "total",
    "touchdown",
    "tourist",
    "toward",
    "tower",
    "town",
    "toy",
    "track",
    "trade",
    "tragic",
    "training",
    "transfer",
    "trap",
    "trash",
    "travel",
    "tray",
    "treat",
    "tree",
    "trek",
    "trend",
    "trial",
    "tribe",
    "trick",
    "trigger",
    "trill",
    "trim",
    "trippi",
    "trophy",
    "trouble",
    "truck",
    "truly",
    "trumpet",
    "trust",
    "truth",
    "try",
    "tube",
    "tuck",
    "tuition",
    "tumble",
    "tuna",
    "tunnell",
    "turkey",
    "turnover",
    "turtle",
    "twelve",
    "twenty",
    "twice",
    "twin",
    "twist",
    "two",
    "type",
    "typical",
    "ugly",
    "umbrella",
    "unable",
    "unaware",
    "unbalanced",
    "uncle",
    "uncover",
    "under",
    "undo",
    "undrafted",
    "unfair",
    "unfold",
    "unhappy",
    "uniform",
    "unique",
    "unitas",
    "universe",
    "unknown",
    "unlimited",
    "unlock",
    "unsportsmanlike",
    "until",
    "untimed",
    "unusual",
    "unveil",
    "upback",
    "update",
    "upgrade",
    "uphold",
    "upman",
    "upon",
    "upper",
    "upset",
    "upshaw",
    "urban",
    "urge",
    "usage",
    "use",
    "used",
    "useful",
    "useless",
    "usual",
    "utility",
    "vacant",
    "vacuum",
    "vague",
    "valid",
    "valley",
    "valve",
    "van",
    "vanilla",
    "vapor",
    "variety",
    "vast",
    "vault",
    "veer",
    "vehicle",
    "velvet",
    "vendor",
    "venture",
    "venue",
    "verb",
    "verify",
    "version",
    "very",
    "vessel",
    "veteran",
    "viable",
    "vibrant",
    "vicious",
    "victory",
    "video",
    "view",
    "vikings",
    "village",
    "vintage",
    "violin",
    "virtual",
    "virus",
    "visa",
    "visibile",
    "visor",
    "visual",
    "vital",
    "vivid",
    "vocal",
    "voice",
    "void",
    "volcano",
    "volume",
    "vote",
    "voyage",
    "wage",
    "wagon",
    "wait",
    "walker",
    "walnut",
    "walsh",
    "want",
    "ward",
    "warfield",
    "warm",
    "warning",
    "warrior",
    "washington",
    "wasp",
    "waste",
    "water",
    "wave",
    "way",
    "weak",
    "wealth",
    "weapon",
    "wear",
    "weasel",
    "weather",
    "web",
    "webster",
    "wedding",
    "weekend",
    "weird",
    "welcome",
    "west",
    "wet",
    "whale",
    "what",
    "wheat",
    "wheel",
    "when",
    "where",
    "whip",
    "whisper",
    "white",
    "wide",
    "width",
    "wife",
    "wilcox",
    "wild",
    "williams",
    "wilson",
    "win",
    "window",
    "wine",
    "wingback",
    "wink",
    "winner",
    "winslow",
    "winter",
    "wire",
    "wisdom",
    "wise",
    "wishbone",
    "witness",
    "wolf",
    "woman",
    "wonder",
    "woodson",
    "wool",
    "word",
    "work",
    "world",
    "worry",
    "worth",
    "wrap",
    "wreck",
    "wrestle",
    "wright",
    "wrist",
    "write",
    "wrong",
    "yards",
    "yarn",
    "yary",
    "year",
    "yellow",
    "you",
    "young",
    "youth",
    "zebra",
    "zero",
    "zimmer",
    "zone",
    "zoo",
    "zoom"
};

#endif /* MNEMONIC_H_ */
