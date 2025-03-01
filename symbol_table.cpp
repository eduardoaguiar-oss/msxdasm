// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// MSXDasm
// Copyright (C) 1999-2025 Eduardo Aguiar
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include "symbol_table.hpp"
#include <cstring>
#include <stdexcept>
#include <unordered_map>

namespace msxdasm
{
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Symbol struct
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
struct Symbol
{
  std::uint16_t addr;
  std::string label;
  std::string comment;
};

} // namespace

namespace msxdasm
{
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Symbol table implementation class
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class symbol_table::impl
{
public:
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Prototypes
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  bool has_symbol (addr_type) const;
  void add_symbol (addr_type, const std::string&, const std::string&);
  std::string get_label (addr_type) const;
  std::string get_comment (addr_type) const;
  void load_def (const std::string&);

private:
  //! \brief Symbols
  std::unordered_map <addr_type, Symbol> symbols_;
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Check if table has a given symbol
//! \param addr Address
//! \return true/false
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool
symbol_table::impl::has_symbol (addr_type pc) const
{
  return symbols_.find (pc) != symbols_.end ();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get symbol label
//! \param addr Address
//! \return Label, if found
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string
symbol_table::impl::get_label (addr_type pc) const
{
  std::string label;

  auto iter = symbols_.find (pc);

  if (iter != symbols_.end ())
    label = iter->second.label;

  return label;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get symbol comment
//! \param addr Address
//! \return Comment, if found
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string
symbol_table::impl::get_comment (addr_type pc) const
{
  std::string comment;

  auto iter = symbols_.find (pc);

  if (iter != symbols_.end ())
    comment = iter->second.comment;

  return comment;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Add symbol to table
//! \param addr Address
//! \param label Label
//! \param comment Comment
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
symbol_table::impl::add_symbol (
  addr_type addr,
  const std::string& label,
  const std::string& comment
)
{
  Symbol symbol;
  symbol.addr = addr;
  symbol.label = label;
  symbol.comment = comment;

  if (label.empty ())
    {
      char buffer[8];
      sprintf (buffer, "l%04x", addr);
      symbol.label = buffer;
    }

  symbols_.try_emplace (addr, symbol);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Load .def file
//! \param path File path
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
symbol_table::impl::load_def (const std::string& path)
{
  FILE *fp = fopen (path.c_str (), "rt");
  if (!fp)
    throw std::runtime_error (strerror (errno));

  constexpr size_t LINE_MAX = 1024;
  std::array <char, LINE_MAX> buffer;

  while (fgets (buffer.data (), LINE_MAX, fp))
    {
      std::string line (buffer.data ());
      const auto pos = line.find_last_not_of ("\r\n\t ");

      if (pos != std::string::npos)
        line.erase (pos + 1);

      Symbol symbol;
      symbol.addr = std::stoi (line.substr (0, 4), nullptr, 16);

      if (line.size () > 5)
        symbol.label = line.substr (5, 6);

      if (line.size () > 12)
        symbol.comment = line.substr (12);

      symbols_[symbol.addr] = symbol;
   }

  fclose (fp);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Constructor
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
symbol_table::symbol_table ()
  : impl_ (std::make_shared <impl> ())
{
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Check if table has a given address
//! \param addr Address
//! \return true/false
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool
symbol_table::has_symbol (addr_type pc) const
{
  return impl_->has_symbol (pc);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get address label
//! \param addr Address
//! \return Label, if found
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string
symbol_table::get_label (addr_type pc) const
{
  return impl_->get_label (pc);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get symbol comment
//! \param addr Address
//! \return Comment, if found
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string
symbol_table::get_comment (addr_type pc) const
{
  return impl_->get_comment (pc);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Add symbol to table
//! \param addr Address
//! \param label Label
//! \param comment Comment
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
symbol_table::add_symbol (
  addr_type addr,
  const std::string& label,
  const std::string& comment
)
{
  impl_->add_symbol (addr, label, comment);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Load .def file
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
symbol_table::load_def (const std::string& path)
{
  impl_->load_def (path);
}

} // namespace msxdasm