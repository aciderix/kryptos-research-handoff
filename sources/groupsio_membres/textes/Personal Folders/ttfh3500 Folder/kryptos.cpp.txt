#include <cctype>
#include <cstdio>
#include <cmath>
#include <stdexcept>
#include <string_view>
#include <string>

#include <io.h>
#include <fcntl.h>
#include <windows.h>

using namespace std;

int WrapIndex(int n, int base) {
	return ((n % base) + base) % base;
}

void PrintFactors(int n) {
	printf("%d: ", n);
	for (int i = 1; i <= sqrt(n); i++)
		if (n % i == 0)
			printf("%s%d x %d", (i > 1) ? ", " : "", i, n / i);
	printf("\n\n");
}

string Normalize(string_view text) {
	string result;
	result.reserve(text.size());
	for (char c : text) {
		if (isalpha(c))
			result += toupper(c);
		else if (c == '?')
			result += c;
	}
	return result;
}

wstring NormalizeW(const wstring& text) {
	wstring result;
	result.reserve(text.length());
	for (wchar_t c : text) {
		if ((c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z'))
			result += towupper(c);
		else if (c >= L'\u0410' && c <= L'\u042F')
			result += c;
		else if (c >= L'\u0430' && c <= L'\u044F')
			result += c - L'\u0430' + L'\u0410';
	}
	return result;
}

class QuagmireIII {
private:
	string key;
	string keyed_alphabet;
	static constexpr string_view ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	string Process(string_view text, bool encode) const {
		string output;
		output.reserve(text.size());
		const size_t base = keyed_alphabet.length();
		size_t key_pos = 0;

		for (char c : text) {
			const size_t input_idx = keyed_alphabet.find(c);
			if (input_idx == string::npos) {
				output += c;
				continue;
			}
			const char key_char = key[key_pos++ % key.length()];
			const size_t key_idx = keyed_alphabet.find(key_char);
			const int shift = encode ? key_idx : -key_idx;
			output += keyed_alphabet[WrapIndex(input_idx + shift, base)];
		}
		return output;
	}
public:
	QuagmireIII(string_view keyword, string_view key) : key(key) {
		keyed_alphabet.reserve(ALPHABET.length());
		for (char c : keyword)
			if (keyed_alphabet.find(c) == string::npos)
				keyed_alphabet += c;
		for (char c : ALPHABET)
			if (keyed_alphabet.find(c) == string::npos)
				keyed_alphabet += c;
	}
	string Encode(string_view plaintext) const {
		return Process(plaintext, true);
	}
	string Decode(string_view ciphertext) const {
		return Process(ciphertext, false);
	}
};

class QuagmireIV {
private:
	wstring key;
	wstring keyed_alphabet;
	wstring keyed_lookup;
	wstring ALPHABET;

	wstring Process(const wstring& text, bool encode) const {
		wstring output;
		output.reserve(text.length());
		const size_t base = keyed_alphabet.length();
		const size_t key_len = key.length();
		size_t key_pos = 0;

		for (wchar_t c : text) {
			const size_t input_idx = encode
				? keyed_lookup.find(c)
				: keyed_alphabet.find(c);
			if (input_idx == wstring::npos) {
				output += c;
				continue;
			}
			const wchar_t key_char = key[key_pos++ % key_len];
			const size_t key_idx = keyed_alphabet.find(key_char);
			const int shift = encode ? key_idx : -key_idx;
			output += encode
				? keyed_alphabet[WrapIndex(input_idx + shift, base)]
				: keyed_lookup[WrapIndex(input_idx + shift, base)];
		}
		return output;
	}
public:
	QuagmireIV(const wstring& keyword, const wstring& key_phrase) {
		ALPHABET = L"АБВГДЕЖЗИЙКЛМНОПРСТУФХШЦЧЩЪЫЬЭЮЯ";
		keyed_lookup = L"АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

		keyed_alphabet.reserve(ALPHABET.length());
		for (wchar_t c : keyword)
			if (keyed_alphabet.find(c) == wstring::npos)
				keyed_alphabet += c;
		for (wchar_t c : ALPHABET)
			if (keyed_alphabet.find(c) == wstring::npos)
				keyed_alphabet += c;

		wstring encoded_key;
		encoded_key.reserve(key_phrase.length());
		for (wchar_t c : key_phrase) {
			const size_t key_idx = ALPHABET.find(c);
			encoded_key += keyed_alphabet[key_idx];
		}
		key = encoded_key;
	}
	QuagmireIV(const wstring& keyword, const wstring& key_phrase, const wstring& lookup_keyword)
		: key(key_phrase) {
		ALPHABET = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		keyed_alphabet.reserve(ALPHABET.length());
		for (wchar_t c : keyword)
			if (keyed_alphabet.find(c) == wstring::npos)
				keyed_alphabet += c;
		for (wchar_t c : ALPHABET)
			if (keyed_alphabet.find(c) == wstring::npos)
				keyed_alphabet += c;

		keyed_lookup.reserve(ALPHABET.length());
		for (wchar_t c : lookup_keyword)
			if (keyed_lookup.find(c) == wstring::npos)
				keyed_lookup += c;
		for (wchar_t c : ALPHABET)
			if (keyed_lookup.find(c) == wstring::npos)
				keyed_lookup += c;
	}
	wstring Encode(const wstring& plaintext) const {
		return Process(plaintext, true);
	}
	wstring Decode(const wstring& ciphertext) const {
		return Process(ciphertext, false);
	}
};

enum Direction {
	CW, CCW
};

class Transposition {
private:
	char** grid = nullptr;
	unsigned int rows = 0;
	unsigned int cols = 0;

	void InitGrid(const string& text, unsigned int num_rows) {
		if (text.length() % num_rows != 0)
			throw invalid_argument("The text length must be a multiple of the number of rows.");
		rows = num_rows;
		cols = text.length() / num_rows;
		grid = new char*[rows];
		for (unsigned int row = 0; row < rows; row++) {
			grid[row] = new char[cols];
			for (unsigned int col = 0; col < cols; col++)
				grid[row][col] = text[row * cols + col];
		}
	}
	void ClearGrid() {
		for (unsigned int row = 0; row < rows; row++)
			delete[] grid[row];
		delete[] grid;
		grid = nullptr;
	}
	void SwapDimensions() {
		unsigned int temp = rows;
		rows = cols;
		cols = temp;
	}
	char** AllocRotated() const {
		char** rotated = new char*[cols];
		for (unsigned int col = 0; col < cols; col++)
			rotated[col] = new char[rows];
		return rotated;
	}
public:
	Transposition(const string& text, unsigned int num_rows) {
		InitGrid(text, num_rows);
	}
	void Resize(unsigned int num_rows) {
		const string text = Fetch();
		ClearGrid();
		InitGrid(text, num_rows);
	}
	void Rotate(Direction dir) {
		char** rotated = AllocRotated();
		for (unsigned int row = 0; row < cols; row++)
			for (unsigned int col = 0; col < rows; col++)
				rotated[row][col] = dir == CW ?
				grid[rows - col - 1][row] : 
				grid[col][cols - row - 1];
		ClearGrid();
		SwapDimensions();
		grid = rotated;
	}
	string Fetch() const {
		string text;
		text.reserve(rows * cols);
		for (unsigned int row = 0; row < rows; row++)
			for (unsigned int col = 0; col < cols; col++)
				text += grid[row][col];
		return text;
	}
	~Transposition() {
		ClearGrid();
	}
};

// https://www.cryptogram.org/downloads/aca.info/ciphers/QuagmireIII.pdf
static void RunTest1() {
	constexpr string_view expected =
	"KRSLWMITJDVIABMRGQMTMLLIVIFUIXRHTNYONVRHHIIIRMCAOVEI";
	const string plaintext = Normalize(
	"The same keyed alphabet is used for plain and cipher alphabets");

	const QuagmireIII cipher("AUTOMOBILE", "HIGHWAY");

	const string encoded = cipher.Encode(plaintext);
	if (encoded != expected)
		throw runtime_error("T1 encoding failed.");

	const string decoded = cipher.Decode(encoded);
	if (decoded != plaintext)
		throw runtime_error("T1 decoding failed.");

	printf("%s\n\n", decoded.c_str());
}

// https://www.cryptogram.org/downloads/aca.info/ciphers/QuagmireIV.pdf
static void RunTest2() {
	constexpr wstring_view expected =
	L"VBMRFCYISPMPBRRHEICXRREIGDX";

	const wstring plaintext = NormalizeW(
	L"This one employs three keywords");

	const QuagmireIV cipher(L"PERCEPTION", L"EXTRA", L"SENORY");

	const wstring encoded = cipher.Encode(plaintext);
	if (encoded != expected)
		throw runtime_error("T2 encoding failed.");

	const wstring decoded = cipher.Decode(encoded);
	if (decoded != plaintext)
		throw runtime_error("T2 decoding failed.");

	wprintf(L"%ls\n\n", decoded.c_str());
}

static void DecodeK1() {
	constexpr string_view expected =
	"EMUFPHZLRFAXYUSDJKZLDKRNSHGNFIVJ"
	"YQTQUXQBQVYUVLLTREVJYQTMKYRDMFD";

	const string plaintext = Normalize(
	"Between subtle shading and the absence of light lies the nuance of iqlusion.");

	const QuagmireIII cipher("KRYPTOS", "PALIMPSEST");

	const string encoded = cipher.Encode(plaintext);
	if (encoded != expected)
		throw runtime_error("K1 encoding failed.");

	const string decoded = cipher.Decode(encoded);
	if (decoded != plaintext)
		throw runtime_error("K1 decoding failed.");

	printf("%s\n\n", decoded.c_str());
}

static void DecodeK2() {
	constexpr string_view expected =
	"VFPJUDEEHZWETZYVGWHKKQETGFQJNCE"
	"GGWHKK?DQMCPFQZDQMMIAGPFXHQRLG"
	"TIMVMZJANQLVKQEDAGDVFRPJUNGEUNA"
	"QZGZLECGYUXUEENJTBJLBQCRTBJDFHRR"
	"YIZETKZEMVDUFKSJHKFWHKUWQLSZFTI"
	"HHDDDUVH?DWKBFUFPWNTDFIYCUQZERE"
	"EVLDKFEZMOQQJLTTUGSYQPFEUNLAVIDX"
	"FLGGTEZ?FKZBSFDQVGOGIPUFXHHDRKF"
	"FHQNTGPUAECNUVPDJMQCLQUMUNEDFQ"
	"ELZZVRRGKFFVOEEXBDMVPNFQXEZLGRE"
	"DNQFMPNZGLFLPMRJQYALMGNUVPDXVKP"
	"DQUMEBEDMHDAFMJGZNUPLGESWJLLAETG";

	const string plaintext = Normalize(
	"It was totally invisible, How's that possible? They used the Earths magnetic field X"
	"The information was gathered and transmitted undergruund to an unknown location X"
	"Does Langley know about this? They should. It's buried out there somewhere X"
	"Who knows the exact location? Only W.W. This was his last message: X"
	"Thirty eight degrees fifty seven minutes six point five seconds north"
	"seventy seven degrees eight minutes forty four seconds west X"
	"Layer two.");

	const QuagmireIII cipher("KRYPTOS", "ABSCISSA");

	const string encoded = cipher.Encode(plaintext);
	if (encoded != expected)
		throw runtime_error("K2 encoding failed.");

	const string decoded = cipher.Decode(encoded);
	if (decoded != plaintext)
		throw runtime_error("K2 decoding failed.");

	printf("%s\n\n", decoded.c_str());
}

static void DecodeK3() {
	const string expected =
	"ENDYAHROHNLSRHEOCPTEOIBIDYSHNAIA"
	"CHTNREYULDSLLSLLNOHSNOSMRWXMNE"
	"TPRNGATIHNRARPESLNNELEBLPIIACAE"
	"WMTWNDITEENRAHCTENEUDRETNHAEOE"
	"TFOLSEDTIWENHAEIOYTEYQHEENCTAYCR"
	"EIFTBRSPAMHHEWENATAMATEGYEERLB"
	"TEEFOASFIOTUETUAEOTOARMAEERTNRTI"
	"BSEDDNIAAHTTMSTEWPIEROAGRIEWFEB"
	"AECTDDHILCEIHSITEGOEAOSDDRYDLORIT"
	"RKLMLEHAGTDHARDPNEOHMGFMFEUHE"
	"ECDMRIPFEIMEHNLSSTTRTVDOHW";

	const string plaintext = Normalize(
	"Slowly, desparatly slowly,"
	"the remains of passage debris that encumbered the lower part of the doorway was removed."
	"With trembling hands I made a tiny breach in the upper left-hand corner."
	"And then, widening the hole a little, I inserted the candle and peered in."
	"The hot air escaping from the chamber caused the flame to flicker,"
	"but presently details of the room within emerged from the mist. X"
	"Can you see anything Q");

	PrintFactors(expected.length());

	Transposition encoder(plaintext, 8);
	encoder.Rotate(CW);
	encoder.Resize(24);
	encoder.Rotate(CW);

	Transposition decoder(expected, 14);
	decoder.Rotate(CCW);
	decoder.Resize(42);
	decoder.Rotate(CCW);

	const string encoded = encoder.Fetch();
	if (encoded != expected)
		throw runtime_error("K3 encoding failed.");

	const string decoded = decoder.Fetch();
	if (decoded != plaintext)
		throw runtime_error("K3 decoding failed.");

	printf("%s\n\n", decoded.c_str());
}

static void DecodeK4() {
	constexpr string_view expected =
	"?OBKR"
	"UOXOGHULBSOLIFBBWFLRVQQPRNGKSSO"
	"TWTQSJQSSEKZZWATJKLUDIAWINFBNYP"
	"VTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
	(void)expected;
	// This part has been left out as an exercise for the reader.
}

// Cyrillic projector
static void DecodeCP1() {
	constexpr wstring_view expected =
	L"ЛТФЕЮТФЯЙЯМПХЦФАЧНЩПВБГЖЧСКЬГГЛЗДЭЙП"
	"ЪКХСЙРЭАФНФПЩВПЕЦРДФАЩШТКСХСЧЫУХХЕЮ"
	"КУМЛЕЧЛЫТОБНЕЯЖЖИЬНЭЗЩЦРЛЫБПНФОИИАБЬ"
	"ПИКЛЕУРЫСМЪШЛЛБХМХЛЖШРАЩРЙЛПЕООЙЙВЦ"
	"ИЪЛБХЦРЫЧСКАРСРВЯЭФКЮФРЮМОЯЗОЛОДЭШРЗУ"
	"ДХМАЭХОЙГЙЮФМЩХХСВИИЗХАГЙЯЬПСИБРРШОМ"
	"КТСУЯГХУЬЛЕУРЫСМЪШСППЯЯЦШУШАЦЧПИМШН"
	"РБЧРЯЫМИУРАДФАИЮЙЫЦЯЛОНУФЖОФШХФЖСБ"
	"ВЪЧДЦСФБМДЭШРЗУДХУРБШТОКЩЪМХПОТОХОЩЧ"
	"ЖАЦДЩРАЮГОЙВРБГЮБЗГЕЖРЙЛПЕООЙЙВЦНЗПГФ"
	"ЦЗАИВЯЮФЛЪЦХСЧЫШЬБЕОМЩШЖТЭДЙОТТФХПР"
	"ПЛОДЭЩРЗУДХКПГФОЦБЩЪММЭКЧЕРЛМКЪЦЦЗЩЛ"
	"ФЦЧЪЩКВНФАЕСДПТДФПРЯЙКЮНХВЦБЮЕИСЧЯЧЦ"
	"ХМЖЛСПРЧУЛЭШЖЫИИMEDUSAИНХЕЗЛЧЗРЗЙКЛ"
	"ППЕВЛЧСХЦЫОЙВРБУДХСВЪГЖЧСКАРСРВЯЭФРЩФ"
	"ЯЦЩПЪЗЫТФОЙЙУСДТЮТВСБРХСПБЩЛШКУВЙЙГЗ"
	"ЙАЧЛЬРЙЭМДЧЧРЬСТНКЙЕКДОБЖБЛШИЫЙБЙИДРР"
	"ЦХОЩЖВЪКБЧКЖНФПШЦЗУЙДЯГАЧЙКУЗФЕЦИЯИ"
	"ЙФЭБЛСДТГЗШРЖЕДФЩЖЙЯНБООЬШФПЮКЗЦУДИ"
	"НХЕОХПОЙАХДЭСБЩЙЖЭШВЪОДЩВУСЛМЩГЖШУД"
	"ИГЛЕШКПУУЕЧДЛСУЦЮТЮНХЪПБУПРЬГИУСБЙЙПЮ"
	"ГАФФЕШБФБМЙПИМЪЮКЩХХТНФЩШПЕЯБЧКК";

	const wstring plaintext = NormalizeW(
	L"ВЫСОЧАЙЪИМ ИСКУССТВОМ В ТАЙНОЙ РАЗВЕДКЕ СПИТАЕТСЯ СПОСОБНОСТЬ РАЗРАБОТАТЬ ИСТОЧНИК КОТОРЫМ"
	"ТЫ БУДЕШЬ ВСЕЦЕЛО РАНПОРЯЖАТЬСЯ И КОНТРОЛИРОВАТЬ. ПОЭТОСУ ТАЙНОЙ РАЗВЕДЫВАТЕЛЬНОЙ СЛУЖБЫ"
	"КОНТРОЛИРУЕМЫЙ ИСТОЧНИК КАК ПРАВИЛО ПОСТАЯЛЯЕТ САМУЮ НАДЕЖНУЭ ИНФОРМАЦИЮ. КОНТРОЛИРУЕМЫМ"
	"СЧИТАЕТСЯ КУПЛЕННЫЙ ИЛИ НАХОДЯЩИЙСЯ В ЛЮБОЙ ДРУГОЙ ЗАВИТИМОСТИ ИСТОЧНИК. ПО ТРАДИЦИИ ЦЕЛЬЭ"
	"ПРОФЕССИОНАЛА РАБОТАЮЩЕГО В ТАЙНОЙ РАЗВЕЫКЕ ВСЕГДЯ БЫЛО ОПУТАТЬ ЛЮБОЙ ПОТЕНЦИАЛЬНО ЦЕННЫЙ"
	"ИСХОЧНИК ИНФОРМАЦИИ ПСИХОЛОГИЧЕСКОЙ СЕТЫО И В ПОДХОДЮЩИЙ МОМЕНТЭ ТУ СЕТЬ ЗАТЯНУТЬ."
	"ВОЗМОЖНОСТЕЙ ДЛ[MEDUSA]Я ЭТОГО НЕ ОЧЕНВ МНОЯО НО ТЕХ РАБОТНИКОВ ИАЙНОЙ СЛУЖБЫ КОТОРЫЕ УСПЕШНО"
	"РАЗРАБАТЫВАЮТ КОНТУОЛИРУЕМЫЕ ИСТОЧНИКИ ИНФОРМАЦИИ ОЖИДАЕТ ЗЧЯЧИТЕЛЬНОЕ ПРОДВИЖЕНИЕ ПО СЛУЖБЕ"
	"И УВАЖЕНИЕ СОСЛУЖИВЦЕВ. ОДНАКЛ НЕОБХОДИМЫЕ ДЛЯ ДОСТИЖЕНИЯ ЭТОЙ ЦЕЛИ СПИСОБ ЗЕЙСТВИЙ И ОБРАЗ"
	"ПОВЕДНИЯ В КОРНЕ ПРОТИВОРЕЧАТ ЭТИКЕ И МОРАЛИ ОБЩЕСТВА В ОБЛАСТИ МЕЖЛИЧНОСТНЫХ ОТНОШЕНИЙ");

	const QuagmireIV cipher(L"ТЕНЬ", L"МЕДУЗА");

	const wstring encoded = cipher.Encode(plaintext);
	if (encoded != expected)
		throw runtime_error("CP1 encoding failed.");

	const wstring decoded = cipher.Decode(encoded);
	if (decoded != plaintext)
		throw runtime_error("CP1 decoding failed.");

	wprintf(L"%ls\n\n", decoded.c_str());
}

static void DecodeCP2() {
	constexpr wstring_view expected =
	L"ЩВЙЩЗЛЮСВЮЙКУКФСЫТЫСВЛСЛЬЗУЦИКЩДРСУРЗ"
	"ХРФЙРЭМРХФЛКФАЙЙКУАЛЩГМЙЖШЪЙЬЩКФНФ"
	"ИДЙРФГКУКАЯЙОУМАТЭТЦКВЕЖОЙИДЦДКГЩФЕЖ"
	"БЮХЛЕССЭСНЩЩХПОЬДЖЙЙЗГЕИТЙМАШЙЙУМФС"
	"ЫТЫСВСИДСДРБФНУОРУШТБЗПЪДЗЯЫААЧКУМАЯХ"
	"ТМЦРИЦЗЩЛЛЕУУПФЖТЭДУХРШЙОРБЭЦЙОПЙЛЪЙ"
	"ТЧШЙАФНПШФЭМБЩЪЖТПДЛРШБШБЧРЖЫМНЧЗЫ"
	"ТЙЖЪСМЪЧДХКЛНЦПЗХРФЙРЭМРХФМЫБВЪЧБШЕФ"
	"БЖТЩВВЪУБЧКЖАЦПЫГШАМИПРЙЪГОЦКЙГРЛЮБУ"
	"СПБЮРРХФТЫХЖТГИЙКАФМТНКЙФФНЦЙЫСЧБШЕ"
	// Antipodes:
	"ФБЖБЛСВТЧРЫЙПТМБОЙМСГУМБП"
	"ЪЦРЙРАЪХКЫХАДГОЩГДЦСТМСГЪЮБЭМЙЩГЯЛ"
	"ОЬЦЧШЮСФЕЖБЮХЛЕЙЬЩХШШГОЭШВЕХОЩЖ"
	"РЮКТПШРАЧТЛБХМЦЯСБЛЬНРГПРУТБЭШВЕСМЫ"
	"ЗФЮЙАЯЗХУФОНСВЩГМЫБВСЮБШСВЦПТЪТХСЙ"
	"РЭАФЭФЙЪФРЯЙРУФРФГРДЭЙЬФЗЩЗАТЧДЛСЖ"
	"ТУЛПЖВЪКБЧКЖТГГЙУАШЙДПДРЬЩЗЩГРШМР"
	"ПОЖЫФЕЛКПНЙРЯЫААЧКУМРФХТМЦРИЦЗЩДР"
	"ШЖДРЗПЪВНЭОЖНФЙЭОЖШФНЙРЙЪХЛШКХЪП"
	"БЭХФЬХОЩГРЖМОЯЗХВЮБЧТЧЭЖЙПЭАБЙИДХХ"
	"ФЩНЩДПВЧОПРПЪЙАПОАТГЦПЛФБШТЧТЙФНП"
	"ЮЦСЮЙКУАХФЛТБКХВРДНСФВКАПРПЛЫГЙХРФ"
	"ЙОАЗПЪЙЛЭСБЕФМЙХПЛЫЬЩХШШГОЭШВЦУКГ";

	const wstring plaintext = NormalizeW(
	L"ОБ ИЗГОТОВЛЕНИИ САХАРОВЫМ ОЧЕРЕДНОГО АНТЮСОВЕТСКОГО 'ОБРАЩЕНИЯ' К ЗАПАДУ И ЕГО ИСПОЛЬЗОВАНИИ"
	"АМЕРИКАНЦАМИ ВО ВРАЖДЕБНЫХ СОВЕТСКОМУ СОЮЗУ ЦЕЛЯХ === В МАЕ 1982 ГОДА АКАДЕМИК САХАРОВ А.Д."
	"ИЗГОТОВИЛ 'ОБРАЩЕНИБ' К 'УЧАСТНИКАМ ПАГУОШСКОЙ КОНФЕРЕНЦИИ', СОДЕРЖАЩЕЕ РЕЗКИЕ АНТИСОВЕТСКИЕ"
	"ОЦЕНКИ ВНУТРЕННЕЙ И ВНЕШНЕЙ ПОЛИТИКИ КПСС Ю СОВЕТСКОГО ПРАВИТЕЛЬСТВА, ОБВИНЕНИЕ СССР В"
	"'УСИЛЕНИИ АРМИИ, ФЛОТА, РАКЕТНОГО АРСЕНАЛА И АВИАЦИИ', 'ВО ВМЗШАТЕЛЬ"

	"СТВЕ ВО ВНТТРЗННИЕ ДЗЛА АФГАНИСТАНА И ПНР'. САХАРОВ ДЕМАГОГИЧЕСКИ ЗАЯВЛЯЕТ, ЧТО СОВЕТСКОЕ"
	"ГОСУДАРСТВО ПРОДОЛЖАЕТ ОСТАВАТЬСЯ 'ЗАКРЫТЫМ ОБЩЕСТВОМ', ПРЕСЛЕДУЕТ 'БОРЦОВ ЗА ПРАВА ЧЕЛОВЕКА'."
	"ПЫТАЕТСЯ СКОМПРОМЕТИРОВАТЬ ШИРОКОЕ АНТИВОЕННОЕ ДВИЖЕНИЕ НА ЗАПАДЕ И ЕГО РУКОВОДИТЕЛЕЙ."
	"ОБВИНПЕТ УЧАСТНИКОВ ПАГУОШСКОГО ДВИЖЕНИЯ В 'СЛЕПОМ СЛЕДОВАНИИ' ПОЛИТИКЕ СССР, ПРОВОЦИРУЕТ"
	"УЧЕНЫХ К ВМЕШАТЕЛЬСТВУ ВО ГНУТРЕННИЕ ДЕЛА НАШЕЙ СТРАНЫ И ВЫСТУПЛЕНИЯТ В ЗАЩИТУ ЛИЦ,"
	"ОСУЖДЕННЫХ ЗА СОВЕРШЕНИЕ ОСОБО ОПАСНЫХ ГОСУДАРСТВЕННЫ");

	const QuagmireIV cipher(L"ТЕНЬ", L"МЕДУЗА");

	const wstring encoded = cipher.Encode(plaintext);
	if (encoded != expected)
		throw runtime_error("CP2 encoding failed.");

	const wstring decoded = cipher.Decode(encoded);
	if (decoded != plaintext)
		throw runtime_error("CP2 decoding failed.");

	wprintf(L"%ls\n\n", decoded.c_str());
}

int main() {
	RunTest1();

	DecodeK1();
	DecodeK2();
	DecodeK3();
	DecodeK4();

	_setmode(_fileno(stdout), _O_U16TEXT);
	RunTest2();

	DecodeCP1();
	DecodeCP2();
	return 0;
}
