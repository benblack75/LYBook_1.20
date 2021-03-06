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

#include <LmbrCentral/Dependency/DependencyMonitor.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TransformBus.h>
#include <LmbrCentral/Shape/ShapeComponentBus.h>
#include <LmbrCentral/Dependency/DependencyNotificationBus.h>
#include <Vegetation/EBuses/AreaNotificationBus.h>
#include <Vegetation/EBuses/AreaBlenderRequestBus.h>
#include <Vegetation/AreaComponentBase.h>

namespace LmbrCentral
{
    template<typename, typename>
    class EditorWrappedComponentBase;
}

namespace Vegetation
{
    class AreaBlenderConfig
        : public AreaConfig
    {
    public:
        AZ_CLASS_ALLOCATOR(AreaBlenderConfig, AZ::SystemAllocator, 0);
        AZ_RTTI(AreaBlenderConfig, "{ED57731E-2821-4AA6-9BD6-9203ED0B6AB0}", AreaConfig);
        static void Reflect(AZ::ReflectContext* context);
        bool m_inheritBehavior = true;
        bool m_propagateBehavior = true;
        AZStd::vector<AZ::EntityId> m_vegetationAreaIds;

        size_t GetNumAreas() const;
        AZ::EntityId GetAreaEntityId(int index) const;
        void RemoveAreaEntityId(int index);
        void AddAreaEntityId(AZ::EntityId entityId);
    };

    static const AZ::Uuid AreaBlenderComponentTypeId = "{899AA751-BC3F-45D8-9D66-07CE72FDC86D}";

    /**
    * Placement logic for combined vegetation areas
    */
    class AreaBlenderComponent
        : public AreaComponentBase
        , private AreaBlenderRequestBus::Handler        
    {
    public:
        template<typename, typename> friend class LmbrCentral::EditorWrappedComponentBase;
        AZ_COMPONENT(AreaBlenderComponent, AreaBlenderComponentTypeId, AreaComponentBase);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void Reflect(AZ::ReflectContext* context);

        AreaBlenderComponent(const AreaBlenderConfig& configuration);
        AreaBlenderComponent() = default;
        ~AreaBlenderComponent() = default;

        //////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;
        bool ReadInConfig(const AZ::ComponentConfig* baseConfig) override;
        bool WriteOutConfig(AZ::ComponentConfig* outBaseConfig) const override;

        //////////////////////////////////////////////////////////////////////////
        // AreaRequestBus
        bool PrepareToClaim(EntityIdStack& stackIds) override;
        void ClaimPositions(EntityIdStack& stackIds, ClaimContext& context) override;
        void UnclaimPosition(const ClaimHandle handle) override;

        // AreaInfoBus
        AZ::Aabb GetEncompassingAabb() const override;
        AZ::u32 GetProductCount() const override;

    protected:
        //////////////////////////////////////////////////////////////////////////
        // AreaBlenderRequestBus
        float GetAreaPriority() const override;
        void SetAreaPriority(float priority) override;
        AreaLayer GetAreaLayer() const override;
        void SetAreaLayer(AreaLayer layer) override;
        AZ::u32 GetAreaProductCount() const override;
        bool GetInheritBehavior() const override;
        void SetInheritBehavior(bool value) override;
        bool GetPropagateBehavior() const override;
        void SetPropagateBehavior(bool value) override;
        size_t GetNumAreas() const override;
        AZ::EntityId GetAreaEntityId(int index) const override;
        void RemoveAreaEntityId(int index) override;
        void AddAreaEntityId(AZ::EntityId entityId) override;

    private:
        AreaBlenderConfig m_configuration;
        LmbrCentral::DependencyMonitor m_dependencyMonitor;
        mutable bool m_isRequestInProgress = false; //prevent recursion in case user attaches cyclic dependences

        void SetupDependencies();
    };
}