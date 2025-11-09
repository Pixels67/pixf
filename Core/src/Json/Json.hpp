#ifndef PIXF_JSON_HPP
#define PIXF_JSON_HPP

#include <optional>
#include <sstream>
#include <string>

#include <boost/json.hpp>

namespace Pixf::Core::Json {
    inline std::string ToPrettyJson(const boost::json::value &json, std::optional<std::string> indent = std::nullopt) {
        using namespace boost;

        std::stringstream ss;

        if (!indent.has_value())
            indent = "";

        switch (json.kind()) {
            case json::kind::object: {
                ss << "{\n";
                indent->append(4, ' ');
                if (auto const &obj = json.get_object(); !obj.empty()) {
                    auto it = obj.begin();
                    for (;;) {
                        ss << *indent << json::serialize(it->key()) << " : ";
                        ss << ToPrettyJson(it->value(), indent);
                        if (++it == obj.end())
                            break;
                        ss << ",\n";
                    }
                }
                ss << "\n";
                indent->resize(indent->size() - 4);
                ss << *indent << "}";
                break;
            }

            case json::kind::array: {
                ss << "[\n";
                indent->append(4, ' ');
                if (auto const &arr = json.get_array(); !arr.empty()) {
                    auto it = arr.begin();
                    for (;;) {
                        ss << *indent;
                        ss << ToPrettyJson(*it, indent);
                        if (++it == arr.end())
                            break;
                        ss << ",\n";
                    }
                }
                ss << "\n";
                indent->resize(indent->size() - 4);
                ss << *indent << "]";
                break;
            }

            case json::kind::string: {
                ss << json::serialize(json.get_string());
                break;
            }

            case json::kind::uint64:
                ss << json.get_uint64();
                break;

            case json::kind::int64:
                ss << json.get_int64();
                break;

            case json::kind::double_:
                ss << json.get_double();
                break;

            case json::kind::bool_:
                if (json.get_bool())
                    ss << "true";
                else
                    ss << "false";
                break;

            case json::kind::null:
                ss << "null";
                break;
        }

        if (indent->empty())
            ss << "\n";

        return ss.str();
    }
} // namespace Pixf::Core::Json

#endif // PIXF_JSON_HPP
