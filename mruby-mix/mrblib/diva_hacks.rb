module Diva
  class << self
    # _model_slug_ をslugとして持つModelクラスを返す。
    # 見つからない場合、nilを返す。
    def Model(model_slug)
      model_dict[model_slug.to_sym]
    end

    private

    def model_dict
      @model ||= Hash.new do |h,k|
        Enumerator.new(ObjectSpace, :each_object, Diva::Model.singleton_class).find do |klass|
          if klass.slug
            h[klass.slug] = klass
          end
          klass.slug == k
        end
      end
    end
  end
end

module Mikutter
  module DivaHacks; end
end
