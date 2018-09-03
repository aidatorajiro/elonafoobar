#pragma once
#include "ui_menu.hpp"

namespace elona
{
namespace ui
{

class UIMenuCharamakeAlias : public UIMenu<std::string>
{
public:
    UIMenuCharamakeAlias(optional<std::string> previous_alias)
        : _previous_alias(previous_alias)
    {
        DIM2(_locked_aliases, 18);
    }

protected:
    virtual bool init();
    virtual void update();
    virtual void draw();
    virtual optional<UIMenuCharamakeAlias::result_type> on_key(
        const std::string& key);

private:
    void _reroll_aliases();

    optional<std::string> _previous_alias;
    elona_vector1<int> _locked_aliases;

    bool _redraw_aliases = false;
};

} // namespace ui
} // namespace elona
