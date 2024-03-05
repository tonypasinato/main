   const Containers::ArrayView<char> pixels = data.exceptPrefix(sizeof(Implementation::TgaHeader));
        Utility::copy(image.pixels(), Containers::StridedArrayView3D<char>{pixels,
            {std::size_t(image.size().y()), std::size_t(image.size().x()), pixelSize}});

        if(image.format() == PixelFormat::RGB8Unorm) {
            for(Vector3ub& pixel: Containers::arrayCast<Vector3ub>(pixels))
                pixel = Math::gather<'b', 'g', 'r'>(pixel);
        } else if(image.format() == PixelFormat::RGBA8Unorm) {
            for(Vector4ub& pixel: Containers::arrayCast<Vector4ub>(pixels))
                pixel = Math::gather<'b', 'g', 'r', 'a'>(pixel);
        }
    }

void ignoreLine(std::istream& in) {
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template<std::size_t size> Math::Vector<size, Float> extractFloatData(const std::string& str, Float* extra = nullptr) {
    std::vector<std::string> data = Utility::String::splitWithoutEmptyParts(str, ' ');
    if(data.size() < size || data.size() > size + (extra ? 1 : 0)) {
        Error() << "Trade::ObjImporter::mesh(): invalid float array size";
        throw 0;
    }
