/// @file
/// @brief Length calculation layer

#pragma once

#include <type_traits>
#include "comms/protocol/ProtocolLayerBase.h"

namespace mqttsn
{

namespace frame
{

namespace layer
{

/// @brief Custom length calculation layer
template <
    typename TField,
    typename TNextLayer,
    typename... TExtraOpts>
class Length : public
    comms::protocol::ProtocolLayerBase<
        TField,
        TNextLayer,
        Length<TField, TNextLayer, TExtraOpts...>
    >
{
    using Base =
        comms::protocol::ProtocolLayerBase<
            TField,
            TNextLayer,
            Length<TField, TNextLayer, TExtraOpts...>
        >;
public:
    /// @brief Smart pointer to message object.
    using MsgPtr = typename Base::MsgPtr;

    static_assert(!std::is_void<MsgPtr>::value,
        "The inner layers must define MsgPtr type");
        
    /// @brief Type of field.
    using Field = typename Base::Field;

    template <typename TMsgPtr, typename TIter, typename TNextLayerReader>
    comms::ErrorStatus doRead(
        Field& field,
        TMsgPtr& msgPtr,
        TIter& iter,
        std::size_t size,
        std::size_t* missingSize,
        TNextLayerReader&& nextLayerReader)
    {
        using IterType = typename std::decay<decltype(iter)>::type ;
        using IterTag = typename std::iterator_traits<IterType>::iterator_category;
        static_assert(
            std::is_base_of<std::random_access_iterator_tag, IterTag>::value,
            "Current implementation of Length requires iterator used for reading to be random-access one.");

        auto es = field.read(iter, size);
        if (es != comms::ErrorStatus::Success) {
            return es;
        }

        auto fromIter = iter;
        auto actualRemainingSize = (size - field.length());
        auto requiredRemainingSize = field.getLengthValue();

        if (actualRemainingSize < requiredRemainingSize) {
            if (missingSize != nullptr) {
                *missingSize = requiredRemainingSize - actualRemainingSize;
            }
            return comms::ErrorStatus::NotEnoughData;
        }

        // not passing missingSize farther on purpose
        es = nextLayerReader.read(msgPtr, iter, requiredRemainingSize, nullptr);
        if (es == comms::ErrorStatus::NotEnoughData) {
            return comms::ErrorStatus::ProtocolError;
        }

        auto consumed =
            static_cast<std::size_t>(std::distance(fromIter, iter));
        if (consumed < requiredRemainingSize) {
            auto diff = requiredRemainingSize - consumed;
            std::advance(iter, diff);
        }
        return es;
    }

    template <typename TMsg, typename TIter, typename TNextLayerWriter>
    comms::ErrorStatus doWrite(
        Field& field,
        const TMsg& msg,
        TIter& iter,
        std::size_t size,
        TNextLayerWriter&& nextLayerWriter) const
    {
        auto wrappedLength = Base::nextLayer().length(msg);
        field.setLengthValue(wrappedLength);
        auto es = field.write(iter, size);
        if (es != comms::ErrorStatus::Success) {
            return es;
        }

        GASSERT(field.length() <= size);
        return nextLayerWriter.write(msg, iter, size - field.length());
    }

    constexpr std::size_t length() const
    {
        return Field::minLength() + Base::nextLayer().length();
    }

    template <typename TMsg>
    std::size_t length(const TMsg& msg) const
    {
        Field fieldTmp;
        auto wrappedLength = Base::nextLayer().length(msg);
        fieldTmp.setLengthValue(wrappedLength);
        return fieldTmp.length() + wrappedLength; 
    }
};

}  // namespace layer

}  // namespace frame

}  // namespace mqttsn


