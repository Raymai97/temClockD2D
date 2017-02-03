#pragma once
#include <string>
#include <exception>
#include <Windows.h>

namespace TemClockD2D {

	enum class ExceptReason {
		FileOpenReadFailed,
		FileOpenWriteFailed,
		InvalidCfgData,
		MxmlSaveFileFailed,
		MxmlOpenFileFailed,
		MxmlNoSuchElement
	};

	inline auto ExceptReasonToString(ExceptReason const r) {
		using R = ExceptReason;
		return
			r == R::FileOpenReadFailed ? L"Failed to open file for read access." :
			r == R::FileOpenWriteFailed ? L"Failed to open file for write access." :
			r == R::InvalidCfgData ? L"Config file contains invalid data." :
			r == R::MxmlNoSuchElement ? L"MiniXML: No such element." :
			r == R::MxmlOpenFileFailed ? L"MiniXML: Failed to open file." :
			r == R::MxmlSaveFileFailed ? L"MiniXML: Failed to save file." :
			L"Undefined";
	}

	class Except {
		ExceptReason	const m_reason;
		std::wstring	const m_customMsg;
		HRESULT			const m_customHr;
		bool			const m_hasCustomMsg;
		bool			const m_hasCustomHr;

	public:
		Except(ExceptReason const & reason) :
			m_reason{ reason },
			m_hasCustomMsg{ false },
			m_customHr{ 0 },
			m_hasCustomHr{ false } {}

		Except(ExceptReason const & reason,
			std::wstring const & customMsg) :
			m_reason{ reason },
			m_customMsg{ customMsg },
			m_hasCustomMsg{ true },
			m_customHr{ 0 },
			m_hasCustomHr{ false } {}

		Except(ExceptReason const & reason,
			HRESULT const customHr) :
			m_reason{ reason },
			m_hasCustomMsg{ false },
			m_customHr{ customHr },
			m_hasCustomHr{ true } {}

		Except(ExceptReason const & reason,
			std::wstring const & customMsg,
			HRESULT const customHr) :
			m_reason{ reason },
			m_customMsg{ customMsg },
			m_hasCustomMsg{ true },
			m_customHr{ customHr },
			m_hasCustomHr{ true } {}

		auto GetReason() const { return m_reason; }
		bool HasCustomMsg() const { return m_hasCustomMsg; }
		bool HasCustomHr() const { return m_hasCustomHr; }
		auto GetCustomMsg() const { return m_customMsg; }
		auto GetCustomHr() const { return m_customHr; }
	};
}
