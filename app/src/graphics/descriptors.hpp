#pragma once

#include "device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace nova {

class nova_DescriptorSetLayout {
 public:
  class Builder {
   public:
    Builder(nova_Device &device) : device{device} {}

    Builder &addBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags,
        uint32_t count = 1);
    std::unique_ptr<nova_DescriptorSetLayout> build() const;

   private:
    nova_Device &device;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
  };

  nova_DescriptorSetLayout(
      nova_Device &device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
  ~nova_DescriptorSetLayout();
  nova_DescriptorSetLayout(const nova_DescriptorSetLayout &) = delete;
  nova_DescriptorSetLayout &operator=(const nova_DescriptorSetLayout &) = delete;

  VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

 private:
  nova_Device &device;
  VkDescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

  friend class nova_DescriptorWriter;
};

class nova_DescriptorPool {
 public:
  class Builder {
   public:
    Builder(nova_Device &device) : device{device} {}

    Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
    Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
    Builder &setMaxSets(uint32_t count);
    std::unique_ptr<nova_DescriptorPool> build() const;

   private:
    nova_Device &device;
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;
  };

  nova_DescriptorPool(
      nova_Device &device,
      uint32_t maxSets,
      VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize> &poolSizes);
  ~nova_DescriptorPool();
  nova_DescriptorPool(const nova_DescriptorPool &) = delete;
  nova_DescriptorPool &operator=(const nova_DescriptorPool &) = delete;

  bool allocateDescriptor(
      const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

  void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

  void resetPool();
  VkDescriptorPool getPool() { return descriptorPool; }

 private:
  nova_Device &device;
  VkDescriptorPool descriptorPool;

  friend class nova_DescriptorWriter;
};

class nova_DescriptorWriter {
 public:
  nova_DescriptorWriter(nova_DescriptorSetLayout &setLayout, nova_DescriptorPool &pool);

  nova_DescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
  nova_DescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

  bool build(VkDescriptorSet &set);
  void overwrite(VkDescriptorSet &set);

 private:
  nova_DescriptorSetLayout &setLayout;
  nova_DescriptorPool &pool;
  std::vector<VkWriteDescriptorSet> writes;
};

}  // namespace nova