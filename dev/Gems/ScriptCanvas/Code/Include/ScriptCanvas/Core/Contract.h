/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#pragma once

#include <AzCore/std/string/string.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/std/functional.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/RTTI.h>

namespace AZ
{
    class ReflectContext;
}

namespace ScriptCanvas
{
    class Slot;
    class Contract;

    //! Function which will be invoked when a slot is created to allow the creation of a slot contract object
    using ContractCreationFunction = AZStd::function<Contract*()>;
    struct ContractDescriptor
    {
        AZ_CLASS_ALLOCATOR(ContractDescriptor, AZ::SystemAllocator, 0);
        AZ_TYPE_INFO(ContractDescriptor, "{C0E3537F-5E6A-4269-A717-17089559F7A1}");
        ContractCreationFunction m_createFunc;

        ContractDescriptor() = default;

        ContractDescriptor(ContractCreationFunction&& createFunc)
            : m_createFunc(AZStd::move(createFunc))
        {}
    };

    class Contract
    {
    public:
        AZ_CLASS_ALLOCATOR(Contract, AZ::SystemAllocator, 0);
        AZ_RTTI(Contract, "{93846E60-BD7E-438A-B970-5C4AA591CF93}");

        Contract() = default;
        virtual ~Contract() = default;

        static void Reflect(AZ::ReflectContext* reflection);

        AZ::Outcome<void, AZStd::string> Evaluate(const Slot& sourceSlot, const Slot& targetSlot) const;

    protected:
        virtual AZ::Outcome<void, AZStd::string> OnEvaluate(const Slot& sourceSlot, const Slot& targetSlot) const = 0;
    };
}
